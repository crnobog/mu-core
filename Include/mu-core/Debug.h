#pragma once

#include "PrimitiveTypes.h"

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
				None,
				C_Str,
				Unsigned
			};

			struct LogArg
			{
				LogArgType m_type = LogArgType::None;
				union
				{
					const char* m_c_str;
					u64 m_uint;
				};

				LogArg() : m_type(LogArgType::None) {}
				LogArg(const char* c_str)
					: m_type(LogArgType::C_Str)
					, m_c_str(c_str)
				{}

				LogArg(i32 i)
					: m_type(LogArgType::Unsigned)
					, m_uint(i)
				{}

				LogArg(u32 u)
					: m_type(LogArgType::Unsigned)
					, m_uint(u)
				{}

				LogArg(size_t s)
					: m_type(LogArgType::Unsigned)
					, m_uint(s)
				{}
			};
		}

		void LogInternal(details::LogLevel level, i32 count, ...);
		
		template<typename ...ARGS>
		void Log(ARGS... args)
		{
			LogInternal(details::LogLevel::Log, sizeof...(ARGS), details::LogArg(args)...);
		}

		template<typename ...ARGS>
		void Err(ARGS... args)
		{
			LogInternal(details::LogLevel::Log, sizeof...(ARGS), details::LogArg(args)...);
		}
	}
}

#define BREAK_OR_CRASH (__debugbreak(),(*(int*)nullptr)++)
#define ASSERT_MSG(...) (mu::dbg::Err(__VA_ARGS__))

#define CHECKF(EXPRESSION, ...) if(!(EXPRESSION)) { ASSERT_MSG(__VA_ARGS__); BREAK_OR_CRASH; }
#define ENSUREF(EXPRESSION, ...) ((EXPRESSION)?true:(ASSERT_MSG(__VA_ARGS__),BREAK_OR_CRASH,false))

#define CHECK(EXPRESSION) CHECKF(EXPRESSION, "Check failed: " #EXPRESSION)
#define ENSURE(EXPRESSION) ENSUREF(EXPRESSION, "Ensure failed: " #EXPRESSION)
