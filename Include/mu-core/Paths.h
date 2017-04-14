#pragma once

#include "mu-core/Ranges.h"
#include "mu-core/Algorithms.h"

namespace mu {
	namespace paths {
		PointerRange<const char> GetExecutablePath();
		PointerRange<const char> GetExecutableDirectory();

		PointerRange<const char> GetDirectory(PointerRange<const char> r);
	}
}
