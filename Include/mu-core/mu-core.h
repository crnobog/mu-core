#pragma once

#ifdef MU_CORE_IMPL

#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <Windows.h>

#endif

#include "PrimitiveTypes.h"
#include "Algorithms.h"
#include "Array.h"
#include "Debug.h"
#include "FileReader.h"
#include "Functors.h"
#include "Math.h"
#include "Metaprogramming.h"
#include "Ranges.h"
#include "RefCountPtr.h"
#include "Scope.h"
#include "String.h"
#include "Utils.h"
