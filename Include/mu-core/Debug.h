#pragma once

#include "mu-core/PrimitiveTypes.h"
#include "mu-core/StringFormat.h"
#include <tuple>

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
		}

		void LogInternal(details::LogLevel level, i32 count, StringFormatArg* args);
		
		template<typename ...ARGS>
		void Log(ARGS... args)
		{
			StringFormatArg wrap_args[] = { StringFormatArg(std::forward<ARGS>(args))... };
			LogInternal(details::LogLevel::Log, sizeof...(ARGS), wrap_args);
		}

		template<typename ...ARGS>
		void Err(ARGS... args)
		{
			StringFormatArg wrap_args[] = { StringFormatArg(std::forward<ARGS>(args))... };
			LogInternal(details::LogLevel::Log, sizeof...(ARGS), wrap_args);
		}
	}
}

#define BREAK_OR_CRASH (__debugbreak(),(*(int*)nullptr)++)
#define ASSERT_MSG(...) (mu::dbg::Err(__VA_ARGS__))

#define CHECKF(EXPRESSION, ...) if(!(EXPRESSION)) { ASSERT_MSG(__VA_ARGS__); BREAK_OR_CRASH; }
#define ENSUREF(EXPRESSION, ...) ((EXPRESSION)?true:(ASSERT_MSG(__VA_ARGS__),BREAK_OR_CRASH,false))

#define CHECK(EXPRESSION) CHECKF(EXPRESSION, "Check failed: " #EXPRESSION)
#define ENSURE(EXPRESSION) ENSUREF(EXPRESSION, "Ensure failed: " #EXPRESSION)
