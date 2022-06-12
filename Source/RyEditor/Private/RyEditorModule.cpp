// Copyright 2020-2022 Solar Storm Interactive

#include "RyEditorModule.h"

#define LOCTEXT_NAMESPACE "RyEditorModule"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void FRyEditorModule::StartupModule()
{

}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void FRyEditorModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRyEditorModule, RyEditor)
DEFINE_LOG_CATEGORY(LogRyEditor);
