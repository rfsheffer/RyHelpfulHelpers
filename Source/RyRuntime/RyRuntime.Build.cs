// Copyright 2020 Sheffer Online Services. All Rights Reserved.

using UnrealBuildTool;

public class RyRuntime : ModuleRules
{
    public RyRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
		PrivatePCHHeaderFile = "RyRuntimePrivatePCH.h";

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
            }
        );
    }
}