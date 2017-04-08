#include "mu-core/mu-core.h"

#include "Windows.h"
#include "stdarg.h"
#include <sstream> // TODO remove

void mu::dbg::LogInternal(mu::dbg::details::LogLevel level, i32 count, ...) {
	std::wostringstream o;

	switch (level) {
	case details::LogLevel::Log:
		break;
	case details::LogLevel::Error:
		break;
	}
	
	va_list args;
	va_start(args, count);
	for (size_t i = 0; i < count; ++i) {
		details::LogArg arg = va_arg(args, details::LogArg);
		switch (arg.m_type) {
		case details::LogArgType::C_Str:
			o << arg.m_c_str;
			break;
		case details::LogArgType::Unsigned:
			o << arg.m_uint;
			break;
		default:
			throw new std::runtime_error("Invalid argument type to log");
		}

		++args;
	}

	o << std::endl;

	OutputDebugStringW(o.str().c_str());
}

#include <stdexcept>
#include <codecvt>
#include <locale>

FileReader::FileReader(void* handle) : m_handle(handle) {}

FileReader FileReader::Open(const char* path) {
	// #TODO: Would rather do this conversion on the stack if possible, or with a custom temp allocator at least
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert{};
	std::wstring wide_path = convert.from_bytes(path);

	HANDLE handle = CreateFile(wide_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	return FileReader(handle);
}

mu::ranges::PointerRange<uint8_t>  FileReader::Read(mu::ranges::PointerRange<uint8_t> dest_range) {
	const uint32_t max_per_call = std::numeric_limits<uint32_t>::max();
	while (!dest_range.IsEmpty()) {
		uint32_t bytes_read = 0;
		const uint32_t call_bytes = max_per_call > dest_range.Size() ? uint32_t(dest_range.Size()) : max_per_call;
		if (!ReadFile(m_handle, static_cast<void*>(&dest_range.Front()), call_bytes, reinterpret_cast<LPDWORD>(&bytes_read), nullptr)) {
			throw std::runtime_error("ReadFile failed");
		}
		dest_range.AdvanceBy(bytes_read);
	}
	return dest_range;
}

int64_t FileReader::GetFileSize() const {
	int64_t size = 0;
	GetFileSizeEx(m_handle, reinterpret_cast<PLARGE_INTEGER>(&size));
	return size;
}


Array<uint8_t> LoadFileToArray(const char* path) {
	FileReader reader = FileReader::Open(path);
	auto arr = Array<uint8_t>::MakeUninitialized(reader.GetFileSize());
	reader.Read(mu::Range(arr));
	return std::move(arr);
}
