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
    Multiple = 0x01
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
    MAX
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

    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    static bool OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                               TArray<FString>& OutFilenames, int32& OutFilterIndex,
                               UPARAM(meta = (Bitmask, BitmaskEnum = ERyOpenFileDialogFlags)) int32 Flags = 0,
                               const FString& FileTypes = TEXT("All Files (*)|*|Text Files (*.txt)|*.txt"));

    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    bool OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName);

    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|DialogHelpers")
    bool SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, TArray<FString>& OutFilenames,
                        UPARAM(meta = (Bitmask, BitmaskEnum = ERyOpenFileDialogFlags)) int32 Flags = 0,
                        const FString& FileTypes = TEXT("All Files (*)|*|Text Files (*.txt)|*.txt"));

    UFUNCTION(BlueprintPure, CallInEditor, Category = "RyEditor|DialogHelpers")
    static FString GetLastDirectory(ERyLastDirectory lastDirectory = ERyLastDirectory::GENERIC_OPEN);
};
