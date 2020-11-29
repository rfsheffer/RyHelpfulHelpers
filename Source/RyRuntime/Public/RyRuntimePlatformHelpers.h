// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimePlatformHelpers.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FRyFileFilter, const FString&, PathString);
DECLARE_DYNAMIC_DELEGATE_RetVal_FourParams(bool, FRyDirectoryVisitor, const FString&, FileOrDirectoryPath, const bool, IsDirectory, const bool, WasFiltered, const int32, DirectoryLevel);
typedef TFunction<bool(const FString& PathString)> RyNativeFileFilterSig;
typedef TFunction<bool(const FString& FileOrDirectoryPath, const bool IsDirectory, const bool WasFiltered, const int32 DirectoryLevel)> RyNativeVisitorSig;

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
    * @param Visitor: (Optional) Called each file or directory found while searching. Return false to halt iteration. Useful for deeper control of iteration. Returns true by default.
    * @param Filter: (Optional) Allows filtering of paths which would be added to the PathsOut array. Returns false by default;
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|Directory", DisplayName = "IterateDirectory")
    static bool RyIterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                   FRyDirectoryVisitor Visitor, FRyFileFilter Filter);

    /**
    * Iterates files in a directory.
    * This is a native and more optimal version which doesn't rely on dynamic delegates.
    * When IterateSubFolders is True, also iterates sub folders after the current folder has been fully iterated.
    * @param DirectoryName: The path to the folder to iterate
    * @param IterateSubFolders: Should sub directories be iterated as well?
    * @param OutType: The type of output in PathsOut.
    * @param PathsOut: Paths collected during iteration, based on OutType setting.
    * @param Visitor: (Optional) Called each file or directory found while searching. Return false to halt iteration. Useful for deeper control of iteration. Returns true by default.
    * @param Filter: (Optional) Allows filtering of paths which would be added to the PathsOut array. Returns false by default;
    */
    static bool IterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                 RyNativeVisitorSig Visitor = nullptr, RyNativeFileFilterSig Filter = nullptr);

	/**
	 * Get file time stamp of file at FilePath
	 * @param filePath - The file path to get a time stamp of
	 * @param isValid - Is the returned text valid? False if the file didn't exist.
	 * @return The file time stamp of file at FilePath
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
	static FORCEINLINE FDateTime GetFileTimeStamp(const FString& filePath, bool& isValid)
	{
    	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		isValid = FileExists(filePath);
		return isValid ? platformFile.GetTimeStamp(*filePath) : FDateTime(0);
	}

	/**
	 * Return the text representing a files time stamp
	 * @param filePath - The path to the file
	 * @param isValid - Is the returned text valid? False if the file didn't exist.
	 * @param longName - Should the returned string be a short DataTime style or long DataTime style
	 * @return The text representing a files time stamp
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
	static FORCEINLINE FText GetFileTimeStampText(const FString& filePath, bool& isValid, const bool longName = false)
	{
    	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		isValid = FileExists(filePath);
		if(!isValid)
		{
			return FText::FromString(TEXT("INVALID"));
		}
		FDateTime FileTimeStamp = platformFile.GetTimeStamp(*filePath);
		const FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
		FileTimeStamp += UTCOffset;
		if(longName)
		{
			return FText::AsDateTime(FileTimeStamp, EDateTimeStyle::Long, EDateTimeStyle::Medium, FText::GetInvariantTimeZone());
		} 
		return FText::AsDateTime(FileTimeStamp, EDateTimeStyle::Short, EDateTimeStyle::Short,FText::GetInvariantTimeZone());
	}

	/**
	 * Delete a file at FilePath
	 * @return True if file delete request was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem")
	static FORCEINLINE bool DeleteFile(const FString& filePath)
	{
    	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		return platformFile.DeleteFile(*filePath);
	}

	/**
	* Delete a directory at directoryPath
	* @param directoryPath - The directory path to delete
	* @param recursive - Delete all files and subdirectories in a directory, then delete the directory itself
	* @return True if directory delete request was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static FORCEINLINE bool DeleteDirectory(const FString& directoryPath, const bool recursive = false)
	{
		IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		if(recursive)
		{
			return platformFile.DeleteDirectoryRecursively(*directoryPath);
		}
		return platformFile.DeleteDirectory(*directoryPath);
	}

	/**
	 * Create a directory, including any parent directories and return true if the directory was created or already existed.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static FORCEINLINE bool CreateDirectoryTree(const FString& directoryTreePath)
	{
		IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		return platformFile.CreateDirectoryTree(*directoryTreePath);
	}

	/**
	* Copy a file from SourcePath to DestinationPath
	* @param sourcePath - The source path to copy from
	* @param destinationPath - The destination to copy the file located at sourcePath to.
	* @param updateTimeStamp - Set to true if you want the timestamp updated to the current time for the file after it is copied.
	* @return True if file copy request was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem", meta=(AdvancedDisplay = "2"))
	static FORCEINLINE bool CopyFile(const FString& sourcePath, const FString& destinationPath, const bool updateTimeStamp = false)
	{
    	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		const bool fileCopied = platformFile.CopyFile(*destinationPath,*sourcePath);
		if(fileCopied && updateTimeStamp)
		{
			platformFile.SetTimeStamp(*destinationPath, FDateTime::Now());
		}
		return fileCopied;
	}

	/**
	* Move a file from SourcePath to DestinationPath
	* @param sourcePath - The source path to move from
	* @param destinationPath - The destination to move the file located at sourcePath to.
	* @param updateTimeStamp - Set to true if you want the timestamp updated to the current time for the file after it is moved.
	* @return True if file move request was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem", meta=(AdvancedDisplay = "2"))
	static FORCEINLINE bool MoveFile(const FString& sourcePath, const FString& destinationPath, const bool updateTimeStamp = false)
	{
    	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		const bool fileMoved = platformFile.MoveFile(*destinationPath, *sourcePath);
		if(fileMoved && updateTimeStamp)
		{
			platformFile.SetTimeStamp(*destinationPath, FDateTime::Now());
		}
		return fileMoved;
	}

	/**
	 * Does a file at FilePath exist?
	 * @return True if a file exists at FilePath
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static FORCEINLINE bool FileExists(const FString& filePath)
	{
		IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		return platformFile.FileExists(*filePath);
	}

	/**
	 * Does a folder exist at DirectoryPath?
	 * @return True if a folder exists at FilePath
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static FORCEINLINE bool FolderExists(const FString& directoryPath)
	{
		IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
		return platformFile.DirectoryExists(*directoryPath);
	}

	/**
	 * Get information about a file system path (This wraps the native call GetStatData)
	 * @param fileSystemPath - Path to a file or directory
	 * @param exists - The path is valid and a file or directory exists there
	 * @param isFile - True if the path is a file
	 * @param isDirectory - True if the path is a folder
	 * @param isReadOnly - True if this file is read-only
	 * @param creationTime - The time the file or directory was originally created, or FDateTime::MinValue if the creation time is unknown
	 * @param accessTime - The time the file or directory was last accessed, or FDateTime::MinValue if the access time is unknown
	 * @param modificationTime - The time the file or directory was last modified, or FDateTime::MinValue if the modification time is unknown
	 * @param fileSize - Size of the file (in bytes), or -1 if the file size is unknown
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
	static void PathInfo(const FString& fileSystemPath,
						 bool& exists,
						 bool& isFile,
						 bool& isDirectory,
						 bool& isReadOnly,
						 FDateTime& creationTime,
						 FDateTime& accessTime,
						 FDateTime& modificationTime,
						 int64& fileSize);
};

FORCEINLINE void URyRuntimePlatformHelpers::PathInfo(const FString& fileSystemPath,
                                                     bool& exists,
                                                     bool& isFile,
                                                     bool& isDirectory,
                                                     bool& isReadOnly,
                                                     FDateTime& creationTime,
                                                     FDateTime& accessTime,
                                                     FDateTime& modificationTime,
                                                     int64& fileSize)
{
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FFileStatData statData = platformFile.GetStatData(*fileSystemPath);
	exists = statData.bIsValid;
	isFile = !statData.bIsDirectory;
	isDirectory = statData.bIsDirectory;
	isReadOnly = statData.bIsReadOnly;
	creationTime = statData.CreationTime;
	accessTime = statData.AccessTime;
	modificationTime = statData.ModificationTime;
	fileSize = statData.FileSize;
}
