#pragma once

// TODO: Can we remove this and use a standard C array?
#include <array>

namespace mu
{
	namespace dbg
	{
		namespace details
		{
			enum class LogLevel
			{
				Log,
				Error
			};

			enum class LogArgType
			{
				C_Str,
				Unsigned
			};

			struct LogArg
			{
				LogArgType m_type;
				union
				{
					const char* m_c_str;
					uint64_t m_uint;
				};

				LogArg(const char* c_str)
					: m_type(LogArgType::C_Str)
					, m_c_str(c_str)
				{}

				LogArg(int32_t i32)
					: m_type(LogArgType::Unsigned)
					, m_uint(i32)
				{}

				LogArg(uint32_t u32)
					: m_type(LogArgType::Unsigned)
					, m_uint(u32)
				{}

				LogArg(size_t s)
					: m_type(LogArgType::Unsigned)
					, m_uint(s)
				{}
			};
		}

		void LogInternal(details::LogLevel level, const details::LogArg*, size_t);
		
		template<typename ...ARGS>
		void LogDispatch(details::LogLevel level, ARGS... args)
		{
			auto arr = std::array<details::LogArg, sizeof...(args)>{ {details::LogArg(args)...}};
			LogInternal(level, arr.data(), arr.size());
		}

		template<typename ...ARGS>
		void Log(ARGS... args)
		{
			LogDispatch(details::LogLevel::Log, std::forward<ARGS>(args)...);
		}

		template<typename ...ARGS>
		void Err(ARGS... args)
		{
			LogDispatch(details::LogLevel::Log, std::forward<ARGS>(args)...);
		}
	}
}

#define BREAK_OR_CRASH (__debugbreak(),(*(int*)nullptr)++)
#define ASSERT_MSG(...) (mu::dbg::Err(__VA_ARGS__))

#define CHECKF(EXPRESSION, ...) if(!(EXPRESSION)) { ASSERT_MSG(__VA_ARGS__); BREAK_OR_CRASH; }
#define ENSUREF(EXPRESSION, ...) ((EXPRESSION)?true:(ASSERT_MSG(__VA_ARGS__),BREAK_OR_CRASH,false))

#define CHECK(EXPRESSION) CHECKF(EXPRESSION, "Check failed: " #EXPRESSION)
#define ENSURE(EXPRESSION) ENSUREF(EXPRESSION, "Ensure failed: " #EXPRESSION)

#ifdef MU_CORE_IMPL
#include <sstream>

void mu::dbg::LogInternal(mu::dbg::details::LogLevel level, const details::LogArg* args, size_t count)
{
	std::wostringstream o;

	switch (level)
	{
	case details::LogLevel::Log:
		break;
	case details::LogLevel::Error:
		break;
	}

	for (size_t i = 0; i < count; ++i)
	{
		switch (args->m_type)
		{
		case details::LogArgType::C_Str:
			o << args->m_c_str;
			break;
		case details::LogArgType::Unsigned:
			o << args->m_uint;
			break;
		default:
			throw new std::runtime_error("Invalid argument type to log");
		}

		++args;
	}

	o << std::endl;

	OutputDebugStringW(o.str().c_str());
}
#endif
