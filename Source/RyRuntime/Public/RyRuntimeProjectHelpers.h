// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeProjectHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions for the project.
*/
UCLASS()
class RYRUNTIME_API URyRuntimeProjectHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

	// Return true if the WITH_EDITOR define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsWithEditor();

	// Return true if the WITH_EDITORONLY_DATA define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsWithEditorOnlyData();

	// Return true if the UE_BUILD_SHIPPING define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static bool IsBuildShipping();

	// Return true if the UE_BUILD_DEBUG define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsBuildDebug();

	// Return true if the UE_BUILD_DEVELOPMENT define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsBuildDevelopment();

	// Return true if the UE_BUILD_TEST define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsBuildTest();

	// Return true if the UE_BUILD_SHIPPING_WITH_EDITOR define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsBuildShippingWithEditor();

	// Whether compiling for dedicated server or not. Define UE_SERVER is true?
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
    static bool IsBuildServer();

	/** The name of the company (author, provider) that created the project. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectCompanyName();

	/** The distinguished name of the company (author, provider) that created the project. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectCompanyDistinguishedName();

	/** The project's copyright and/or trademark notices. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectCopyrightNotice();

	/** The project's description text. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectDescription();

	/** The project's homepage URL. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectHomepage();

	/** The project's licensing terms. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectLicensingTerms();

	/** The project's privacy policy. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectPrivacyPolicy();

	/** The project's unique identifier. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FGuid GetProjectID();

	/** The project's name. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectName();

	/** The project's version number. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectVersion();

	/** The project's support contact information. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FString GetProjectSupportContact();

	/** The project's title as displayed on the window title bar (can include the tokens {GameName}, {PlatformArchitecture}, {BuildConfiguration} or {RHIName}, which will be replaced with the specified text) */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FText GetProjectDisplayedTitle();

	/** Additional data to be displayed on the window title bar in non-shipping configurations (can include the tokens {GameName}, {PlatformArchitecture}, {BuildConfiguration} or {RHIName}, which will be replaced with the specified text) */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|ProjectHelpers")
	static FText GetProjectDebugTitleInfo();
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsWithEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsWithEditorOnlyData()
{
#if WITH_EDITORONLY_DATA
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsBuildShipping()
{
#if UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsBuildDebug()
{
#if UE_BUILD_DEBUG
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsBuildDevelopment()
{
#if UE_BUILD_DEVELOPMENT
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsBuildTest()
{
#if UE_BUILD_TEST
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsBuildShippingWithEditor()
{
#if UE_BUILD_SHIPPING_WITH_EDITOR
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimeProjectHelpers::IsBuildServer()
{
#if UE_SERVER
	return true;
#else
	return false;
#endif
}
