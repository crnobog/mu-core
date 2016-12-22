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

	mu::ranges::PointerRange<uint8_t> Read(mu::ranges::PointerRange<uint8_t> dest_range);

	bool IsValidFile() const { return m_handle != nullptr; }
	int64_t GetFileSize() const;
};

#ifdef MU_CORE_IMPL
#include <stdexcept>
#include <codecvt>
#include <locale>

FileReader::FileReader(void* handle) : m_handle(handle)
{
}

FileReader FileReader::Open(const char* path)
{
	// #TODO: Would rather do this conversion on the stack if possible, or with a custom temp allocator at least
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert{};
	std::wstring wide_path = convert.from_bytes(path);

	HANDLE handle = CreateFile(wide_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	return FileReader(handle);
}

mu::ranges::PointerRange<uint8_t>  FileReader::Read(mu::ranges::PointerRange<uint8_t> dest_range)
{
	const uint32_t max_per_call = std::numeric_limits<uint32_t>::max();
	while (!dest_range.IsEmpty())
	{
		uint32_t bytes_read = 0;
		const uint32_t call_bytes = max_per_call > dest_range.Size() ? uint32_t(dest_range.Size()) : max_per_call;
		if (!ReadFile(m_handle, static_cast<void*>(&dest_range.Front()), call_bytes, reinterpret_cast<LPDWORD>(&bytes_read), nullptr))
		{
			throw std::runtime_error("ReadFile failed");
		}
		dest_range.AdvanceBy(bytes_read);
	}
	return dest_range;
}

int64_t FileReader::GetFileSize() const
{
	int64_t size = 0;
	GetFileSizeEx(m_handle, reinterpret_cast<PLARGE_INTEGER>(&size));
	return size;
}


Array<uint8_t> LoadFileToArray(const char* path)
{
	FileReader reader = FileReader::Open(path);
	auto arr = Array<uint8_t>::MakeUninitialized(reader.GetFileSize());
	reader.Read(mu::Range(arr));
	return std::move(arr);
}

#endif