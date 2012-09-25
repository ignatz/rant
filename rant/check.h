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
typename std::enable_if<std::is_integral<T>::value>::type
check(T val)
{
	static_assert(Max::value >= Min::value, "Max must be >= Min");

	if (!(val >= Min::value))
		throw RANT_UNDERFLOW_ERROR(val, Min::value);
	else if (!(val < Max::value))
		throw RANT_OVERFLOW_ERROR(val, Max::value);
}

template<typename T, typename Max, typename Min>
typename std::enable_if<std::is_floating_point<T>::value>::type
check(T val)
{
	static_assert(std::ratio_greater_equal<Max, Min>::value,
		"Max must be >= Min");

	if (!(val >= static_cast<T>(Min::num) / Min::den))
		throw RANT_UNDERFLOW_ERROR(val, static_cast<T>(Min::num) / Min::den);
	else if (!(val < static_cast<T>(Max::num) / Max::den))
		throw RANT_OVERFLOW_ERROR(val, static_cast<T>(Max::num) / Max::den);
}

} // namespace rant

#undef RANT_UNDERFLOW_ERROR
#undef RANT_OVERFLOW_ERROR
