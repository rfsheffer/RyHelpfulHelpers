// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Runtime/Core/Public/HAL/Platform.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeFileHelpers.generated.h"

/**
 * A handle to a file
 * If this object is garbage collected or destroyed
 */
UCLASS(BlueprintType)
class RYRUNTIME_API URyFileHandle : public UObject
{
	GENERATED_BODY()
public:

	URyFileHandle() : Handle(nullptr), CanRead(false), CanWrite(false)
	{
	}
	
	virtual void BeginDestroy() override;

	/**
	 * Change the current write or read position.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	bool Seek(const int64 newPosition);

	/**
	 * Seek the file handle by numBytes from the end
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	bool SeekFromEnd(const int64 numBytes);

	/**
	 * Seek the file handle to the start
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	bool SeekToStart();

	/**
	 * Seek the file handle to the end
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	bool SeekToEnd();

	/**
	 * Read from the file
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	bool Read(TArray<uint8>& bytesTo, const int64 numBytes);

	/**
	 * Write to the file
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	bool Write(const TArray<uint8>& bytesOut);

	/**
	 * Return the size of the file
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|FileHandle")
	int64 Size() const;

	/**
	 * Closes the file handle. No further operations can be performed with this handle once this is called.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHandle")
	void Close();

private:

	// The handle to the file. If this object is destroyed the file handle will go as well.
	IFileHandle* Handle;

	bool CanRead;
	bool CanWrite;

	friend class URyRuntimeFileHelpers;
};

/**
 * Helpers related to files
 */
UCLASS()
class RYRUNTIME_API URyRuntimeFileHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	 * Open a file handle for reading and writing to a file. Use the file handle object to seek, read, write, etc.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHelpers")
	static URyFileHandle* OpenFileHandle(UObject* outer, const FString filePath, const bool forRead, const bool forWrite, bool& success);

	/**
	 * Write an array of bytes to a file
	 * @param filePath The full path to the file to write to
	 * @param allowOverwrite If true, if the file exists it will be overwritten, or the function will return false
	 * @param bytesOut The bytes to write out to the file
	 * @return True if success
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHelpers")
	static bool WriteBytesToFile(const FString filePath, const bool allowOverwrite, const TArray<uint8>& bytesOut);

	/**
	 * Read all bytes from a file and put them into bytesIn
	 * @param filePath The full path to the file to write to
	 * @param bytesIn The bytes to read in from the file
	 * @return True if success
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHelpers")
	static bool ReadAllBytesFromFile(const FString filePath, TArray<uint8>& bytesIn);

	/**
	 * Read bytes from a file and put them into bytesIn
	 * The default offset and numBytes will read the entire file. Change the parameters accordingly.
	 * @param filePath The full path to the file to write to
	 * @param bytesIn The bytes to read in from the file
	 * @param offset The offset in bytes into the file
	 * @param numBytes The number of bytes at the offset to read
	 * @return True if success
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|FileHelpers")
	static bool ReadBytesFromFile(const FString filePath, TArray<uint8>& bytesIn, const int64 offset = 0, const int64 numBytes = 99999999999);
};
