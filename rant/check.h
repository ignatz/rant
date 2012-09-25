#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>
#include <string>

#include "rant/util.h"

#ifdef RANT_LIGHTWEIGHT_EXCEPTIONS
#define RANT_UNDERFLOW_ERROR(VAL, MIN) std::underflow_error("")
#define RANT_OVERFLOW_ERROR(VAL, MAX)  std::overflow_error("")
#else
#define RANT_UNDERFLOW_ERROR(VAL, MIN) rant::underflow(VAL, MIN)
#define RANT_OVERFLOW_ERROR(VAL, MAX)  rant::overflow(VAL, MAX)
#endif // RANT_LIGHTWEIGHT_EXCEPTIONS


namespace rant {

template<typename T>
inline
std::underflow_error
underflow(T val, T min)
{
	std::string s("underflow error: ");
	s += std::to_string(val) + " < min(";
	s += std::to_string(min) + ")";
	return std::underflow_error(s);
}

template<typename T>
inline
std::overflow_error
overflow(T val, T max)
{
	std::string s("overflow error: ");
	s += std::to_string(val) + " >= max(";
	s += std::to_string(max) + ")";
	return std::overflow_error(s);
}

template<typename T, typename Max, typename Min>
inline
void check(T val)
{
	if (!(val >= value<T, Min>()))
		throw RANT_UNDERFLOW_ERROR(val, (value<T, Min>()));
	else if (!(val < value<T, Max>()))
		throw RANT_OVERFLOW_ERROR(val, (value<T, Max>()));
}

} // namespace rant

#undef RANT_UNDERFLOW_ERROR
#undef RANT_OVERFLOW_ERROR
