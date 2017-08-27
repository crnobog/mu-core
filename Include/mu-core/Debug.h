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
			if constexpr (sizeof...(args) > 0) {
				StringFormatArg wrap_args[] = { StringFormatArg(std::forward<ARGS>(args))... };
				LogInternal(details::LogLevel::Log, sizeof...(ARGS), wrap_args);
			}
		}
	}
}

#define BREAK_OR_CRASH (__debugbreak(),(*(int*)nullptr)++)

template<typename... TS>
__forceinline bool Assert(bool e, TS... ts) {
	if (!e) {
		mu::dbg::Err(std::forward<TS>(ts)...);
		BREAK_OR_CRASH; // TODO: Do not crash if no debugger is attached
		return false;
	}
	return true;
}