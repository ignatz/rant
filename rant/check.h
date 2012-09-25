#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>
#include <string>
#include <typeinfo>

#include "rant/util.h"

template<typename T>
inline
std::underflow_error
underflow(T val, T min)
{
	std::string s("type(");
	s += typeid(T).name();
	s += ") overflow error: ";
	s += std::to_string(val) + " < min(";
	s += std::to_string(min) + ")";
	return std::underflow_error(s);
}

template<typename T>
inline
std::overflow_error
overflow(T val, T max)
{
	std::string s("type(");
	s += typeid(T).name();
	s += ") overflow error: ";
	s += std::to_string(val) + " >= max(";
	s += std::to_string(max) + ")";
	return std::overflow_error(s);
}

namespace rant {

template<typename T, typename Max, typename Min>
typename std::enable_if<std::is_integral<T>::value>::type
check(T val)
{
	static_assert(Max::value >= Min::value, "Max must be >= Min");

	if (!(val >= Min::value))
		throw underflow(val, Min::value);
	else if (!(val < Max::value))
		throw overflow(val, Max::value);
}

template<typename T, typename Max, typename Min>
typename std::enable_if<std::is_floating_point<T>::value>::type
check(T val)
{
	static_assert(std::ratio_greater_equal<Max, Min>::value,
		"Max must be >= Min");

	if (!(val >= static_cast<T>(Min::num) / Min::den))
		throw underflow(val, static_cast<T>(Min::num) / Min::den);
	else if (!(val < static_cast<T>(Max::num) / Max::den))
		throw overflow(val, static_cast<T>(Max::num) / Max::den);
}

} // namespace rant
