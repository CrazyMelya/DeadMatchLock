// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "SharedTypes.h"
#include "AbilitySystem/DMLAbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Components/WidgetComponent.h"
#include "UI/PlayerInfo.h"
#include "DMLCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADMLCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WC_PlayerInfo;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UDMLAbilitySystemComponent* AbilitySystemComponent;
	
	ADMLCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Fire")
	FVector GetFirePointLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Movement")
	FVector2D GetMovementVector();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Look")
	FVector2D GetLookVector();

	UFUNCTION(BlueprintCallable, DisplayName="PauseMontage")
	void BP_PauseMontage();

	UFUNCTION(BlueprintCallable, DisplayName="ResumeMontage")
	void BP_ResumeMontage();

protected:
	virtual void StartAbility(EAbilityInputID InputID);
	virtual void EndAbility(EAbilityInputID InputID);

	virtual bool CanJumpInternal_Implementation() const override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayEffect> InitEffectClass;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base")
	void Die(AActor* Killer);
	virtual void Die_Implementation(AActor* Killer);

	UFUNCTION(Client, Reliable)
	void Die_Client();

	UFUNCTION(NetMulticast, Reliable)
	void Die_Multicast();

	UFUNCTION()
	void OnEffectRemoved(const FActiveGameplayEffect& Effect) const;
	
	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;

	UPROPERTY(BlueprintReadOnly, Category = "PlayerInfo")
	UPlayerInfo* PlayerInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement")
	float WallJumpForce = 1000.f;

	// virtual void ResetJumpState() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	bool WallJumpIsAllowed();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void WallJump(FVector JumpDirection);
	
private:
	UFUNCTION(NetMulticast, Unreliable)
	void PauseMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void ResumeMontage();
};

