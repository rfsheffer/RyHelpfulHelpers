// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeProjectHelpers.h"
#include "Runtime/EngineSettings/Classes/GeneralProjectSettings.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectCompanyName()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.CompanyName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectCompanyDistinguishedName()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.CompanyDistinguishedName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectCopyrightNotice()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.CopyrightNotice;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectDescription()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.Description;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectHomepage()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.Homepage;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectLicensingTerms()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.LicensingTerms;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectPrivacyPolicy()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.PrivacyPolicy;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FGuid URyRuntimeProjectHelpers::GetProjectID()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectID;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectName()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectVersion()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectVersion;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeProjectHelpers::GetProjectSupportContact()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.SupportContact;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FText URyRuntimeProjectHelpers::GetProjectDisplayedTitle()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectDisplayedTitle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FText URyRuntimeProjectHelpers::GetProjectDebugTitleInfo()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectDebugTitleInfo;
}
