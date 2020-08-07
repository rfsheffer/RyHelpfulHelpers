// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

using UnrealBuildTool;

public class RyRuntime : ModuleRules
{
    public RyRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
            new string[]
            {
                "RyRuntime/Private",
            }
        );
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "Engine",
				"NavigationSystem",
			}
        );
    }
}