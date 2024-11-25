// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Launch/Resources/Version.h"

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

UENUM(BlueprintType)
enum class ERyNetworkConnectionType : uint8
{
	/**
	* Enumerates the network connection types
	*/
	Unknown,
    None,
    AirplaneMode,
    Cell,
    WiFi,
    WiMAX,
    Bluetooth,
    Ethernet,
};

UENUM(BlueprintType)
enum class ERyDeviceScreenOrientation : uint8
{
	/** The orientation is not known */
	Unknown,

    /** The orientation is portrait with the home button at the bottom */
    Portrait,

    /** The orientation is portrait with the home button at the top */
    PortraitUpsideDown,

    /** The orientation is landscape with the home button at the right side */
    LandscapeLeft,

    /** The orientation is landscape with the home button at the left side */
    LandscapeRight,

    /** The orientation is as if place on a desk with the screen upward */
    FaceUp,

    /** The orientation is as if place on a desk with the screen downward */
    FaceDown,
#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27)
	/** The orientation is portrait, oriented upright with the sensor */
	PortraitSensor,

	/** The orientation is landscape, oriented upright with the sensor */
	LandscapeSensor,
#endif
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
    * @param DirectoryName The path to the folder to iterate
    * @param IterateSubFolders Should sub directories be iterated as well?
    * @param OutType The type of output in PathsOut.
    * @param PathsOut Paths collected during iteration, based on OutType setting.
    * @param Visitor Called each file or directory found while searching. Return false to halt iteration. Useful for deeper control of iteration. Returns true by default.
    * @param Filter Allows filtering of paths which would be added to the PathsOut array. Returns false by default;
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|Directory", DisplayName = "IterateDirectory")
    static bool RyIterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                   FRyDirectoryVisitor Visitor, FRyFileFilter Filter);

    /**
    * Iterates files in a directory.
    * This is a native and more optimal version which doesn't rely on dynamic delegates.
    * When IterateSubFolders is True, also iterates sub folders after the current folder has been fully iterated.
    * @param DirectoryName The path to the folder to iterate
    * @param IterateSubFolders Should sub directories be iterated as well?
    * @param OutType The type of output in PathsOut.
    * @param PathsOut Paths collected during iteration, based on OutType setting.
    * @param Visitor (Optional) Called each file or directory found while searching. Return false to halt iteration. Useful for deeper control of iteration. Returns true by default.
    * @param Filter (Optional) Allows filtering of paths which would be added to the PathsOut array. Returns false by default;
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
	static FDateTime GetFileTimeStamp(const FString& filePath, bool& isValid);

	/**
	 * Return the text representing a files time stamp
	 * @param filePath - The path to the file
	 * @param isValid - Is the returned text valid? False if the file didn't exist.
	 * @param longName - Should the returned string be a short DataTime style or long DataTime style
	 * @return The text representing a files time stamp
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
	static FText GetFileTimeStampText(const FString& filePath, bool& isValid, const bool longName = false);

	/**
	 * Delete a file at FilePath
	 * @return True if file delete request was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem")
	static bool DeleteFile(const FString& filePath);

	/**
	* Delete a directory at directoryPath
	* @param directoryPath - The directory path to delete
	* @param recursive - Delete all files and subdirectories in a directory, then delete the directory itself
	* @return True if directory delete request was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static bool DeleteDirectory(const FString& directoryPath, const bool recursive = false);

	/**
	 * Create a directory, including any parent directories and return true if the directory was created or already existed.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static bool CreateDirectoryTree(const FString& directoryTreePath);

	/**
	* Copy a file from SourcePath to DestinationPath
	* @param sourcePath - The source path to copy from
	* @param destinationPath - The destination to copy the file located at sourcePath to.
	* @param updateTimeStamp - Set to true if you want the timestamp updated to the current time for the file after it is copied.
	* @return True if file copy request was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem", meta=(AdvancedDisplay = "2"))
	static bool CopyFile(const FString& sourcePath, const FString& destinationPath, const bool updateTimeStamp = false);

	/**
	* Move a file from SourcePath to DestinationPath
	* @param sourcePath - The source path to move from
	* @param destinationPath - The destination to move the file located at sourcePath to.
	* @param updateTimeStamp - Set to true if you want the timestamp updated to the current time for the file after it is moved.
	* @return True if file move request was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers|FileSystem", meta=(AdvancedDisplay = "2"))
	static bool MoveFile(const FString& sourcePath, const FString& destinationPath, const bool updateTimeStamp = false);

	/**
	 * Does a file at FilePath exist?
	 * @return True if a file exists at FilePath
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static bool FileExists(const FString& filePath);

	/**
	 * Does a folder exist at DirectoryPath?
	 * @return True if a folder exists at FilePath
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers|FileSystem")
    static bool FolderExists(const FString& directoryPath);

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

	// Return true if the PLATFORM_DESKTOP define is set
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
    static bool IsPlatformDesktop();

	/**
	* Retrieve a environment variable from the system
	*
	* @param VariableName The name of the variable (ie "Path")
	* @param VariableOut The variable
	* @return True if the variable was found and not empty
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool GetEnvironmentVariable(const FString& VariableName, FString& VariableOut);

	/**
	* Sets an environment variable to the local process's environment
	*
	* @param VariableName The name of the variable (ie "Path")
	* @param ValueToSet The string to set the variable to.
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void SetEnvironmentVariable(const FString& VariableName, const FString& ValueToSet);

	/** Return true if a debugger is present */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
    static bool IsDebuggerPresent();

	/** Break into the debugger, if IsDebuggerPresent returns true, otherwise do nothing  */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
    static void DebugBreak();

	/**
	* Uses cpuid instruction to get the vendor string
	*
	* @return	CPU vendor name
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static FString GetCPUVendor();

	/**
	* Uses cpuid instruction to get the CPU brand string
	*
	* @return	CPU brand string
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static FString GetCPUBrand();

	/**
	* Returns the CPU chipset if known
	*
	* @return	CPU chipset string (or "Unknown")
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static FString GetCPUChipset();

	/**
	* @return primary GPU brand string
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static FString GetPrimaryGPUBrand();

	/**
	* @return	"DeviceMake|DeviceModel" if possible, and "CPUVendor|CPUBrand" otherwise, optionally returns "DeviceMake|DeviceModel|CPUChipset" if known
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static FString GetDeviceMakeAndModel();

	/**
	* Gets the OS Version and OS Subversion.
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers", DisplayName = "Get OS Versions")
	static void GetOSVersions(FString& OS_VersionLabel, FString& OS_SubVersionLabel);

	/**
	* Gets a string representing the numeric OS version (as opposed to a translated OS version that GetOSVersions returns).
	* The returned string should try to be brief and avoid newlines and symbols, but there's technically no restriction on the string it can return.
	* If the implementation does not support this, it should return an empty string.
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers", DisplayName = "Get OS Version")
	static FString GetOSVersion();

	/** Retrieves information about the total number of bytes and number of free bytes for the specified disk path. */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool GetDiskTotalAndFreeSpace(const FString& InPath, int64& TotalNumberOfBytes, int64& NumberOfFreeBytes);

	/**
	* Platform specific function for adding a named event that can be viewed in external tool
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void BeginNamedEvent(const struct FColor& Color, const FString& Text);

	/**
	* Platform specific function for closing a named event that can be viewed in external tool
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void EndNamedEvent();

	/** Platform specific function for adding a named custom stat that can be viewed in external tool */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void CustomNamedStat(const FString& Text, const float Value, const FString& Graph, const FString& Unit);

	/**
	* Profiler color stack - this overrides the color for named events with undefined colors (e.g stat namedevents)
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void BeginProfilerColor(const struct FColor& Color);
	
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void EndProfilerColor();

	/** Sends a message to a remote tool, and debugger consoles */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void LowLevelOutputDebugString(const FString& Message);

	/** Copies text to the operating system clipboard. */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void ClipboardCopy(const FString& Str);

	/** Pastes in text from the operating system clipboard. */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void ClipboardPaste(FString& Dest);

	/**
	* return the number of hardware CPU cores
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static int32 NumberOfCores();

	/**
	* return the number of logical CPU cores
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static int32 NumberOfCoresIncludingHyperthreads();

	/**
	* Get the timezone identifier for this platform, or an empty string if the default timezone calculation will work.
	* @note This should return either an Olson timezone (eg, "America/Los_Angeles") or an offset from GMT/UTC (eg, "GMT-8:00").
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers", DisplayName = "Get Time Zone ID")
	static FString GetTimeZoneId();

	/**
	* Gets the current battery level.
	*
	* @return the battery level between 0 and 100.
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static int32 GetBatteryLevel();

	// Is the device is low power mode?
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool IsInLowPowerMode();

	/**
	* Returns the current device temperature level.
	* Level is a relative value that is platform dependent. Lower is cooler, higher is warmer.
	* Level of -1 means Unimplemented.
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static float GetDeviceTemperatureLevel();

	/**
	* Return an ordered list of target platforms this runtime can support (ie Android_DXT, Android
	* would mean that it prefers Android_DXT, but can use Android as well)
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static void GetValidTargetPlatforms(TArray<FString>& TargetPlatformNames);

	/**
	* Returns whether WiFi connection is currently active
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool HasActiveWiFiConnection();

	/**
	* Returns the type of network connection for the platform
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static ERyNetworkConnectionType GetNetworkConnectionType();

	/**
	* Returns whether the given platform feature is currently available (for instance, Metal is only available in IOS8 and with A7 devices)
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool HasPlatformFeature(const FString& FeatureName);

	/**
	* Returns whether the platform is running on battery power or not.
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool IsRunningOnBattery();

	/**
	* Returns the orientation of the device: e.g. Portrait, LandscapeRight.
	* @see ERyDeviceScreenOrientation
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static ERyDeviceScreenOrientation GetDeviceOrientation();
	
	/**
	 * Change the orientation of the device: e.g. Portrait, LandscapeRight.
	 * NOTE: This currently only supports Android on Engine version 4.27 or greater.
	 * @see ERyDeviceScreenOrientation
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|PlatformHelpers")
	static void SetDeviceOrientation(ERyDeviceScreenOrientation NewDeviceOrientation);

	/**
	* Returns the device volume if the device is capable of returning that information.
	*  -1 : Unknown
	*   0 : Muted
	* 100 : Full Volume
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static int32 GetDeviceVolume();

	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool FileExistsInPlatformPackage(const FString& RelativePath);

	/// Does this platform require cooked data?
	UFUNCTION(BlueprintPure, Category = "RyRuntime|PlatformHelpers")
	static bool PlatformRequiresCookedData();

	/**
	 * Only valid on android devices, returns the internal storage path. This path is always accessible at runtime
	 * and should be where you put the games extra runtime generated files.
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|FileHelpers")
	static FString GetAndroidInternalStoragePath();

	/**
	 * Only valid on android devices, returns the external storage path. This path is only valid if the application
	 * has the permissions WRITE_EXTERNAL_STORAGE or READ_EXTERNAL_STORAGE set (one or the other depending on your needs)
	 * and the user as given the application permission using the permission granting prompt.
	 * This is usually used when the user can use their own files to add something to the game like mods, music, art.
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|FileHelpers")
	static FString GetAndroidExternalStoragePath();

    /**
     * @brief Gets the android no backup files dir if is Android platform and the API >= 21
     * This is the directory you can store files in internally which will not be backed up to the cloud.
     * This is a useful place to put your games cache files which could be large and should not take up backup space.
     * NOTE: If the API version is less than 21 the output dir will be the android internal storage path.
     * @param isValid Is the return path valid?
     * @return If is the Android platform and the API >= 21, the no backup files dir. isValid will be false otherwise.
     */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|FileHelpers")
	static FString GetAndroidNoBackupFilesDir(bool& isValid);

	/**
	 * Get the raw, undilated delta time for the application
	 * NOTE: If WorldContextObject is included, the delta will be clamped to WorldSettings Min and MaxUndilatedFrameTime
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|AppHelpers", meta=(AdvancedDisplay="1"))
	static float GetApplicationDeltaTime(const UObject* WorldContextObject = nullptr);
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::IsPlatformDesktop()
{
#if PLATFORM_DESKTOP
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::GetEnvironmentVariable(const FString& VariableName, FString& VariableOut)
{
	VariableOut = FPlatformMisc::GetEnvironmentVariable(*VariableName);
	return !VariableOut.IsEmpty();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::SetEnvironmentVariable(const FString& VariableName, const FString& ValueToSet)
{
	FPlatformMisc::SetEnvironmentVar(*VariableName, *ValueToSet);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::IsDebuggerPresent()
{
	return FPlatformMisc::IsDebuggerPresent();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::DebugBreak()
{
	UE_DEBUG_BREAK();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetCPUVendor()
{
	return FPlatformMisc::GetCPUVendor();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetCPUBrand()
{
	return FPlatformMisc::GetCPUBrand();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetCPUChipset()
{
	return FPlatformMisc::GetCPUChipset();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetPrimaryGPUBrand()
{
	return FPlatformMisc::GetPrimaryGPUBrand();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetDeviceMakeAndModel()
{
	return FPlatformMisc::GetDeviceMakeAndModel();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::GetOSVersions(FString& OS_VersionLabel, FString& OS_SubVersionLabel)
{
	FPlatformMisc::GetOSVersions(OS_VersionLabel, OS_SubVersionLabel);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetOSVersion()
{
	return FPlatformMisc::GetOSVersion();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::GetDiskTotalAndFreeSpace(const FString& InPath, int64& TotalNumberOfBytes, int64& NumberOfFreeBytes)
{
	uint64 total, num;
	const bool success = FPlatformMisc::GetDiskTotalAndFreeSpace(InPath, total, num);
	if(success && total <= MAX_int64 && num <= MAX_int64)
	{
		TotalNumberOfBytes = static_cast<int64>(total);
		NumberOfFreeBytes = static_cast<int64>(num);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::BeginNamedEvent(const FColor& Color, const FString& Text)
{
	FPlatformMisc::BeginNamedEvent(Color, *Text);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::EndNamedEvent()
{
	FPlatformMisc::EndNamedEvent();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::CustomNamedStat(const FString& Text, const float Value, const FString& Graph, const FString& Unit)
{
	FPlatformMisc::CustomNamedStat(*Text, Value, *Graph, *Unit);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::BeginProfilerColor(const FColor& Color)
{
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 5) || ENGINE_MAJOR_VERSION < 5
	FPlatformMisc::BeginProfilerColor(Color);
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::EndProfilerColor()
{
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 5) || ENGINE_MAJOR_VERSION < 5
	FPlatformMisc::EndProfilerColor();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::LowLevelOutputDebugString(const FString& Message)
{
	FPlatformMisc::LowLevelOutputDebugString(*Message);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE int32 URyRuntimePlatformHelpers::NumberOfCores()
{
	return FPlatformMisc::NumberOfCores();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE int32 URyRuntimePlatformHelpers::NumberOfCoresIncludingHyperthreads()
{
	return FPlatformMisc::NumberOfCoresIncludingHyperthreads();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE FString URyRuntimePlatformHelpers::GetTimeZoneId()
{
	return FPlatformMisc::GetTimeZoneId();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE int32 URyRuntimePlatformHelpers::GetBatteryLevel()
{
	return FPlatformMisc::GetBatteryLevel();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::IsInLowPowerMode()
{
	return FPlatformMisc::IsInLowPowerMode();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE float URyRuntimePlatformHelpers::GetDeviceTemperatureLevel()
{
	return FPlatformMisc::GetDeviceTemperatureLevel();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE void URyRuntimePlatformHelpers::GetValidTargetPlatforms(TArray<FString>& TargetPlatformNames)
{
    FPlatformMisc::GetValidTargetPlatforms(TargetPlatformNames);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::HasActiveWiFiConnection()
{
    return FPlatformMisc::HasActiveWiFiConnection();
}

static_assert(ERyNetworkConnectionType::Ethernet == static_cast<ERyNetworkConnectionType>(ENetworkConnectionType::Ethernet), "ENetworkConnectionType misalignment!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE ERyNetworkConnectionType URyRuntimePlatformHelpers::GetNetworkConnectionType()
{
    return static_cast<ERyNetworkConnectionType>(FPlatformMisc::GetNetworkConnectionType());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::HasPlatformFeature(const FString& FeatureName)
{
    return FPlatformMisc::HasPlatformFeature(*FeatureName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::IsRunningOnBattery()
{
    return FPlatformMisc::IsRunningOnBattery();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE ERyDeviceScreenOrientation URyRuntimePlatformHelpers::GetDeviceOrientation()
{
    return static_cast<ERyDeviceScreenOrientation>(FPlatformMisc::GetDeviceOrientation());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE int32 URyRuntimePlatformHelpers::GetDeviceVolume()
{
    return FPlatformMisc::GetDeviceVolume();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE bool URyRuntimePlatformHelpers::FileExistsInPlatformPackage(const FString& RelativePath)
{
	return FPlatformMisc::FileExistsInPlatformPackage(RelativePath);
}
