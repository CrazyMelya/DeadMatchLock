// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DeadMatchLock : ModuleRules
{
	public DeadMatchLock(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"AdvancedWidgets",
			"OnlineServicesNull", "AnimGraphRuntime", "AnimGraphRuntime"
		});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks",
			"UMG",
			"DeveloperSettings",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"OnlineSubsystemSteam",
			"AdvancedSessions"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(EngineDirectory, "Plugins/Online/OnlineServices/Source/OnlineServicesInterface/Public")
		});

	}
}
