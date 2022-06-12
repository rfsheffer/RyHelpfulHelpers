// Copyright 2020-2022 Solar Storm Interactive

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
