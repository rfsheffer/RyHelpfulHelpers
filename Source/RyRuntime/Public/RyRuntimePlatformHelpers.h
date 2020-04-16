// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimePlatformHelpers.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FRyFileFilter, const FString&, PathString);
DECLARE_DYNAMIC_DELEGATE_RetVal_ThreeParams(bool, FRyDirectoryVisitor, const FString&, FileOrDirectoryPath, const bool, IsDirectory, const int32, DirectoryLevel);
typedef TFunction<bool(const FString& PathString)> RyNativeFileFilterSig;
typedef TFunction<bool(const FString& FileOrDirectoryPath, const bool IsDirectory, const int32 DirectoryLevel)> RyNativeVisitorSig;

UENUM(BlueprintType)
enum class ERyIterateDirectoryOut : uint8
{
    /** Store file and directory paths */
    FilesAndDirectories = 0,

    /** Store file paths only */
    FilesOnly = 1,

    /** Store directory paths only */
    DirectoriesOnly = 2,
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions for platform.
*/
UCLASS()
class RYRUNTIME_API URyRuntimePlatformHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /**
    * Iterates files in a directory.
    * When IterateSubFolders is True, also iterates sub folders after the current folder has been fully iterated.
    * @param DirectoryName: The path to the folder to iterate
    * @param IterateSubFolders: Should sub directories be iterated as well?
    * @param OutType: The type of output in PathsOut.
    * @param PathsOut: Paths collected during iteration, based on OutType setting.
    * @param visitor: (Optional) Called each file or directory found while searching. Return false to halt iteration. Useful for deeper control of iteration. Returns true by default.
    * @param Filter: (Optional) Allows filtering of paths which would be added to the PathsOut array. Returns false by default;
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
    static bool RyIterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                   FRyDirectoryVisitor Visitor, FRyFileFilter Filter);

    /**
    * Iterates files in a directory.
    * When IterateSubFolders is True, also iterates sub folders after the current folder has been fully iterated.
    * @param DirectoryName: The path to the folder to iterate
    * @param IterateSubFolders: Should sub directories be iterated as well?
    * @param OutType: The type of output in PathsOut.
    * @param PathsOut: Paths collected during iteration, based on OutType setting.
    * @param visitor: (Optional) Called each file or directory found while searching. Return false to halt iteration. Useful for deeper control of iteration. Returns true by default.
    * @param Filter: (Optional) Allows filtering of paths which would be added to the PathsOut array. Returns false by default;
    */
    static bool IterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                 RyNativeVisitorSig Visitor = nullptr, RyNativeFileFilterSig Filter = nullptr);
};
