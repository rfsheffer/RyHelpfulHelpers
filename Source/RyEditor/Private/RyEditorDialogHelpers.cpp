// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyEditorDialogHelpers.h"

#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "Framework/Application/SlateApplication.h"

#include "Misc/MessageDialog.h"

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorDialogHelpers::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                                            TArray<FString>& OutFilenames, int32& OutFilterIndex, int32 Flags, const FString& FileTypes)
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if(DesktopPlatform)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        if(!ParentWindowWindowHandle)
        {
            return false;
        }
        return DesktopPlatform->OpenFileDialog(ParentWindowWindowHandle,
                                               DialogTitle,
                                               DefaultPath,
                                               DefaultFile,
                                               FileTypes,
                                               Flags,
                                               OutFilenames,
                                               OutFilterIndex);
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorDialogHelpers::OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName)
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if(DesktopPlatform)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        if(!ParentWindowWindowHandle)
        {
            return false;
        }
        return DesktopPlatform->OpenDirectoryDialog(ParentWindowWindowHandle,
                                                    DialogTitle,
                                                    DefaultPath,
                                                    OutFolderName);
    }

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorDialogHelpers::SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                                            TArray<FString>& OutFilenames, int32 Flags, const FString& FileTypes)
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if(DesktopPlatform)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        if(!ParentWindowWindowHandle)
        {
            return false;
        }
        return DesktopPlatform->SaveFileDialog(ParentWindowWindowHandle,
                                               DialogTitle,
                                               DefaultPath,
                                               DefaultFile,
                                               FileTypes,
                                               Flags,
                                               OutFilenames);
    }

    return false;
}

static_assert(ERyLastDirectory::MAX == (ERyLastDirectory)ELastDirectory::MAX, "Last Directory Enum changed. Update ERyLastDirectory to match ELastDirectory!");

//--------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyEditorDialogHelpers::GetLastDirectory(ERyLastDirectory lastDirectory)
{
    ELastDirectory::Type convertType = (ELastDirectory::Type)lastDirectory;
    return FEditorDirectories::Get().GetLastDirectory(convertType);
}

static_assert(ERyAppReturnType::Continue == (ERyAppReturnType)EAppReturnType::Continue, "EAppReturnType changed. Update ERyAppReturnType to match EAppReturnType!");
static_assert(ERyAppMsgType::YesNoYesAll == (ERyAppMsgType)EAppMsgType::YesNoYesAll, "EAppMsgType changed. Update ERyAppMsgType to match EAppMsgType!");

//--------------------------------------------------------------------------------------------------------------------
/**
*/
ERyAppReturnType URyEditorDialogHelpers::OpenMessageDialog(ERyAppMsgType MessageType, const FText& Message, const FText& OptTitle)
{
    const FText* optTitle = nullptr;
    if(!OptTitle.IsEmpty())
    {
        optTitle = &OptTitle;
    }

    return (ERyAppReturnType)FMessageDialog::Open((EAppMsgType::Type)MessageType, EAppReturnType::Cancel, Message, optTitle);
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorDialogHelpers::OpenDebugMessageDialog(const FText& Message, const FText& OptTitle)
{
    const FText* optTitle = nullptr;
    if(!OptTitle.IsEmpty())
    {
        optTitle = &OptTitle;
    }

    FMessageDialog::Debugf(Message, optTitle);
}
