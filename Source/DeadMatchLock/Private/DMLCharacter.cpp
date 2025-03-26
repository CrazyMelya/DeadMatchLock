// Copyright Epic Games, Inc. All Rights Reserved.

#include "DMLCharacter.h"

#include "DMLGameMode.h"
#include "GamePlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Binding/States/WidgetStateRegistration.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADeadMatchLockCharacter

ADMLCharacter::ADMLCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.0f, 50.0f, 75.0f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	AbilitySystemComponent = CreateDefaultSubobject<UDMLAbilitySystemComponent>("AbilitySystemComponent");
}

UAbilitySystemComponent* ADMLCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ADMLCharacter::BeginPlay()
{
	// Call the base class 
	Super::BeginPlay();

	
}

void ADMLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AbilitySystemComponent)
	{
		if (HasAuthority())
		{
			if (UCharactersAttributeSet* Attributes = NewObject<UCharactersAttributeSet>(this, UCharactersAttributeSet::StaticClass()))
			{
				AbilitySystemComponent->AddSpawnedAttribute(Attributes);
				Attributes->OnOutOfHealth.AddUObject(this, &ThisClass::Die);
			}
			auto InitEffect = InitEffectClass->GetDefaultObject<UGameplayEffect>();
			AbilitySystemComponent->ApplyGameplayEffectToSelf(InitEffect, 0, AbilitySystemComponent->MakeEffectContext());
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TAG_STUNNED), EGameplayTagEventType::NewOrRemoved)
				.AddUObject(this, &ThisClass::OnStunnedTagChanged);
			AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnEffectRemoved);
		}
	}
}

void ADMLCharacter::OnStunnedTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		DisableInput(Cast<APlayerController>(GetController()));
	}
	else
	{
		EnableInput(Cast<APlayerController>(GetController()));
	}
}

void ADMLCharacter::OnEffectRemoved(const FActiveGameplayEffect& Effect)
{
	if (Effect.Spec.Def->GetGrantedTags().HasTag(FGameplayTag::RequestGameplayTag(TAG_STUNNED)) &&
		AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TAG_NEEDRELOAD)))
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TAG_RELOAD))), true);
	}
}

void ADMLCharacter::Die_Implementation(AActor* Killer)
{
	if (HasAuthority())
	{
		AbilitySystemComponent->RemoveAllActiveGameplayEffects();
		auto PlayerController = Cast<AGamePlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->UnPossess();
		}
		Die_Client();
		Die_Multicast();
		if (ADMLGameMode* GameMode = GetWorld()->GetAuthGameMode<ADMLGameMode>())
		{
			GameMode->NotifyPlayerDeath(PlayerController, Killer);
		}
	}
}

void ADMLCharacter::Die_Multicast_Implementation()
{
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bAutoActivate = true;
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SetLifeSpan(10.0f);
}

void ADMLCharacter::Die_Client_Implementation()
{
	if (auto PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->UnPossess();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADMLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	// if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	// 	{
	// 		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	// 	}
	// }
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AbilitySystemComponent->AbilityDataAsset)
		{
			auto InitAbilitiesData = AbilitySystemComponent->AbilityDataAsset->Abilities;
			for (auto InitAbilityData : InitAbilitiesData)
			{
				for (uint8 Bit = 1; Bit != 0; Bit <<= 1)
				{
					if (InitAbilityData.ActivateTriggerEvent & Bit)
					{
						EnhancedInputComponent->BindAction(InitAbilityData.InputAction, static_cast<ETriggerEvent>(Bit), this, &ADMLCharacter::StartAbility, InitAbilityData.InputID);
					}
					if (InitAbilityData.DeactivateTriggerEvent & Bit)
					{
						EnhancedInputComponent->BindAction(InitAbilityData.InputAction, static_cast<ETriggerEvent>(Bit), this, &ADMLCharacter::EndAbility, InitAbilityData.InputID);
					}
				}
			}
		}

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADMLCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADMLCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADMLCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADMLCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADMLCharacter::StartAbility(EAbilityInputID InputID)
{
	AbilitySystemComponent->PressInputID(static_cast<int32>(InputID));
}

void ADMLCharacter::EndAbility(EAbilityInputID InputID)
{
	AbilitySystemComponent->ReleaseInputID(static_cast<int32>(InputID));
}

bool ADMLCharacter::CanJumpInternal_Implementation() const
{
	bool bJumpIsAllowed = GetCharacterMovement()->CanAttemptJump();

	if (bJumpIsAllowed)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			if (JumpCurrentCount == 0 && GetCharacterMovement()->IsFalling())
			{
				bJumpIsAllowed = JumpCurrentCount + 1 < AbilitySystemComponent->GetNumericAttribute(UCharactersAttributeSet::GetJumpMaxCountAttribute());
			}
			else
			{
				bJumpIsAllowed = JumpCurrentCount < AbilitySystemComponent->GetNumericAttribute(UCharactersAttributeSet::GetJumpMaxCountAttribute());
			}
		}
		else
		{
			// Only consider JumpKeyHoldTime as long as:
			// A) The jump limit hasn't been met OR
			// B) The jump limit has been met AND we were already jumping
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bJumpIsAllowed = bJumpKeyHeld &&
				((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		}
	}

	return bJumpIsAllowed;
}
