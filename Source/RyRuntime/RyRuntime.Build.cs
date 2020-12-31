// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

using UnrealBuildTool;
using System.IO;

public class RyRuntime : ModuleRules
{
    public RyRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// Turn this off if you want to dis-include dangerous functions from blueprint.
		// This blocks functions which access engine internals which only advanced engine users should consider.
		PrivateDefinitions.Add("RY_INCLUDE_DANGEROUS_FUNCTIONS=1");
		
		//OptimizeCode = CodeOptimization.Never;

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
				"AIModule",
				"UMG",
				"ApplicationCore",
				"Voice",
			}
        );
        
        if (Target.Platform.IsInGroup(UnrealPlatformGroup.Windows))
        {
	        AddEngineThirdPartyPrivateStaticDependencies(Target, "DirectSound");
	        
	        PrivateIncludePaths.AddRange(
		        new string[]
		        {
			        Path.GetFullPath(Path.Combine(EngineDirectory, "Source/Runtime/Online/Voice/Private")),
		        }
	        );
        }
    }
}
