// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Modules/ModuleManager.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class FRyRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogRyRuntime, Log, All);
