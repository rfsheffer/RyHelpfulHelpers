// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorDialogHelpers.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class ERyOpenFileDialogFlags : uint8
{
    // No flags
    None = 0x00,
    // Allow multiple file selections
    Multiple = 0x01,
};

UENUM(BlueprintType)
enum class ERyLastDirectory : uint8
{
    UNR,
    BRUSH,
    FBX,
    FBX_ANIM,
    GENERIC_IMPORT,
    GENERIC_EXPORT,
    GENERIC_OPEN,
    GENERIC_SAVE,
    MESH_IMPORT_EXPORT,
    WORLD_ROOT,
    LEVEL,
    PROJECT,
    NEW_ASSET,
    MAX,
};

/**
* Enumerates message dialog return types.
*/
UENUM(BlueprintType)
enum class ERyAppReturnType : uint8
{
	No,
	Yes,
	YesAll,
	NoAll,
	Cancel,
	Ok,
	Retry,
	Continue,
};

/**
* Enumerates supported message dialog button types.
*/
UENUM(BlueprintType)
enum class ERyAppMsgType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	YesNoCancel,
	CancelRetryContinue,
	YesNoYesAllNoAll,
	YesNoYesAllNoAllCancel,
	YesNoYesAll,
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to system dialogs
*/
UCLASS()
class RYEDITOR_API URyEditorDialogHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // Open a file open dialog box for selecting a file to open
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    static bool OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                               TArray<FString>& OutFilenames, int32& OutFilterIndex,
                               UPARAM(meta = (Bitmask, BitmaskEnum = ERyOpenFileDialogFlags)) int32 Flags = 0,
                               const FString& FileTypes = TEXT("All Files (*)|*|Text Files (*.txt)|*.txt"));

    // Open a directory select dialog for selecting a directory to use
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    static bool OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName);

    // Open a save file dialog box for setting a filename or overwrite file path
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    static bool SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, TArray<FString>& OutFilenames,
                        UPARAM(meta = (Bitmask, BitmaskEnum = ERyOpenFileDialogFlags)) int32 Flags = 0,
                        const FString& FileTypes = TEXT("All Files (*)|*|Text Files (*.txt)|*.txt"));

    // Get the last directory path for a specific type of directory access. Good to pass this value into DefaultPath for OpenFileDialog, SaveFileDialog and OpenDirectoryDialog
    UFUNCTION(BlueprintPure, CallInEditor, Category = "RyEditor|DialogHelpers")
    static FString GetLastDirectory(ERyLastDirectory lastDirectory = ERyLastDirectory::GENERIC_OPEN);

    // Open a generic message dialog
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    static ERyAppReturnType OpenMessageDialog(ERyAppMsgType MessageType, const FText& Message, const FText& OptTitle);

    // Open a debug message dialog
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    static void OpenDebugMessageDialog(const FText& Message, const FText& OptTitle);
};
