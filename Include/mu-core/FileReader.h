#pragma once

#include <cstdint>

#include "Array.h"

Array<uint8_t> LoadFileToArray(const char* path);

class FileReader
{
	void* m_handle = nullptr;

	FileReader(void* handle);
public:

	static FileReader Open(const char* path);

	mu::PointerRange<uint8_t> Read(mu::PointerRange<uint8_t> dest_range);

	bool IsValidFile() const { return m_handle != nullptr; }
	int64_t GetFileSize() const;
};
