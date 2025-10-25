// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pathfinding_Demo_CPP : ModuleRules
{
	public Pathfinding_Demo_CPP(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Pathfinding_Demo_CPP",
			"Pathfinding_Demo_CPP/Variant_Platforming",
			"Pathfinding_Demo_CPP/Variant_Platforming/Animation",
			"Pathfinding_Demo_CPP/Variant_Combat",
			"Pathfinding_Demo_CPP/Variant_Combat/AI",
			"Pathfinding_Demo_CPP/Variant_Combat/Animation",
			"Pathfinding_Demo_CPP/Variant_Combat/Gameplay",
			"Pathfinding_Demo_CPP/Variant_Combat/Interfaces",
			"Pathfinding_Demo_CPP/Variant_Combat/UI",
			"Pathfinding_Demo_CPP/Variant_SideScrolling",
			"Pathfinding_Demo_CPP/Variant_SideScrolling/AI",
			"Pathfinding_Demo_CPP/Variant_SideScrolling/Gameplay",
			"Pathfinding_Demo_CPP/Variant_SideScrolling/Interfaces",
			"Pathfinding_Demo_CPP/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
