// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Runtime/Core/Public/Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRyEditor, Log, All);

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class FRyEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
