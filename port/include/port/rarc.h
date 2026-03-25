/**
 * port/include/port/rarc.h
 * RARC / JKR archive format reader — public API.
 */
#pragma once
#include <stddef.h>

namespace tp::rarc {

/** Opaque archive handle. */
struct Archive;

/**
 * Open an archive from a memory buffer (no copy is made — caller owns the
 * buffer and must keep it alive for the lifetime of the Archive handle).
 */
Archive* Open(const void* rawData, size_t size);

/** Open an archive file from disk.  The file is read entirely into memory. */
Archive* OpenFile(const char* path);

/** Close and free an archive handle. */
void Close(Archive* handle);

/**
 * Retrieve a file from the archive by path (case-insensitive).
 * Returns a pointer into the archive's internal buffer and sets *outSize.
 * Returns nullptr if the path does not exist or is a directory.
 */
const void* GetFile(Archive* handle, const char* path, size_t* outSize);

/** Returns true if the given path exists as a file in the archive. */
bool HasFile(Archive* handle, const char* path);

} // namespace tp::rarc
