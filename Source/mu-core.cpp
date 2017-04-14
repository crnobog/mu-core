#define MU_CORE_IMPL
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

mu::PointerRange<uint8_t>  FileReader::Read(mu::PointerRange<uint8_t> dest_range) {
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

namespace mu {
	StringFormatArg::StringFormatArg(const char* c_str)
		: m_type(StringFormatArgType::C_Str)
		, m_c_str(c_str) {}

	StringFormatArg::StringFormatArg(const String_T<char>& str) {
		if (str.IsEmpty()) {
			m_type = StringFormatArgType::None;
		}
		else {
			m_type = StringFormatArgType::C_Str;
			m_c_str = str.GetRaw();
		}
	}

	StringFormatArg::StringFormatArg(int32_t i32)
		: m_type(StringFormatArgType::Unsigned)
		, m_uint(i32) {}

	StringFormatArg::StringFormatArg(uint32_t u32)
		: m_type(StringFormatArgType::Unsigned)
		, m_uint(u32) {}

	StringFormatArg::StringFormatArg(size_t s)
		: m_type(StringFormatArgType::Unsigned)
		, m_uint(s) {}


	String WideStringToUTF8(PointerRange<const wchar_t> in) {
		std::codecvt_utf8_utf16<wchar_t> conv;
		std::mbstate_t mb{};
		char out_buf[128];
		String s;
		while (!in.IsEmpty()) {
			const wchar_t* in_next = nullptr;
			char* out_next = nullptr;
			std::codecvt_base::result res = conv.out(mb, &in.Front(), &in.Front() + in.Size(), in_next,
				out_buf, out_buf + ArraySize(out_buf), out_next);
			CHECK(res != std::codecvt_base::noconv);
			if (res == std::codecvt_base::error) {
				return String{};
			}

			s.Append(Range(out_buf, out_next));

			if (res == std::codecvt_base::ok) {
				break;
			}
		}
		return s;
	}

	namespace paths {
		PointerRange<const char> GetDirectory(PointerRange<const char> r) {
			auto end = FindLast(r, [](const char c) { return c == '/' || c == '\\'; });
			if (end.IsEmpty()) {
				// return empty, must be filename only?
				return PointerRange<const char>{ };
			}
			return PointerRange<const char>{ &r.Front(), &end.Front() + 1 };
		}

		PointerRange<const char> GetExecutablePath() {
			struct Initializer {
				String path;
				Initializer() {
					wchar_t local_buffer[1024];
					u32 size = GetModuleFileNameW(nullptr, local_buffer, (u32)ArraySize(local_buffer));
					if (size) {
						path = WideStringToUTF8(Range(local_buffer, size));
					}
				}
			};
			static Initializer init;
			return Range(init.path);
		}
		PointerRange<const char> GetExecutableDirectory() {
			return GetDirectory(GetExecutablePath());
		}

	}
}



