// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPU1_Cabral_Juan : ModuleRules
{
	public TPU1_Cabral_Juan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TPU1_Cabral_Juan",
			"TPU1_Cabral_Juan/Variant_Platforming",
			"TPU1_Cabral_Juan/Variant_Platforming/Animation",
			"TPU1_Cabral_Juan/Variant_Combat",
			"TPU1_Cabral_Juan/Variant_Combat/AI",
			"TPU1_Cabral_Juan/Variant_Combat/Animation",
			"TPU1_Cabral_Juan/Variant_Combat/Gameplay",
			"TPU1_Cabral_Juan/Variant_Combat/Interfaces",
			"TPU1_Cabral_Juan/Variant_Combat/UI",
			"TPU1_Cabral_Juan/Variant_SideScrolling",
			"TPU1_Cabral_Juan/Variant_SideScrolling/AI",
			"TPU1_Cabral_Juan/Variant_SideScrolling/Gameplay",
			"TPU1_Cabral_Juan/Variant_SideScrolling/Interfaces",
			"TPU1_Cabral_Juan/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
