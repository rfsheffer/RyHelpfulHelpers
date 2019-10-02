// Copyright 2019 Ryan Sheffer. All Rights Reserved.

#pragma once

#include "Runtime/Core/Public/Modules/ModuleManager.h"

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
