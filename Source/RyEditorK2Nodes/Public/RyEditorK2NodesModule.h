// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Runtime/Core/Public/Modules/ModuleManager.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class FRyEditorK2NodesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
