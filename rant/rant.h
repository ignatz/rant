#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/range.h"
#include "rant/int.h"
#include "rant/float.h"
#include "rant/check.h"

namespace rant {
namespace debug {

#ifndef RANT_DISABLE
using namespace rant;
#else

#if __cplusplus != 201103L
#error disabling rant requires a newer compiler
#endif

#pragma message "rant's debug::ranges are disabled"

template<typename T,
	T Max    = T(),
	T Min    = T(),
	typename Check  = void>
using integral_range = T;

template<typename T,
	typename Max    = void,
	typename Min    = void,
	typename Check  = void>
using floating_point_range = T;

template<typename T,
	typename Max    = void,
	typename Min    = void,
	typename Check  = void>
using range = T;
#endif // RANT_DISABLE

} // debug
} // rant
