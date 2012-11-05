#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>
#include <string>

#include "rant/util.h"

#ifdef RANT_LIGHTWEIGHT_EXCEPTIONS
#define RANT_UNDERFLOW_ERROR(VAL, MIN) std::underflow_error("underflow error")
#define RANT_OVERFLOW_ERROR(VAL, MAX)  std::overflow_error("overflow error")
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


template<typename T, typename Max, typename Min,
	typename Enable = void>
struct throw_on_error;

template<typename T, typename Max, typename Min>
struct throw_on_error<T, Max, Min, typename std::enable_if<
	!std::is_unsigned<T>::value ||
	(value<T, Min>() > 0)>::type>
{
	T operator() (T val)
	{
		if RANT_UNLIKELY((val < value<T, Min>()))
			throw RANT_UNDERFLOW_ERROR(val, (value<T, Min>()));
		else if RANT_UNLIKELY((val > value<T, Max>()))
			throw RANT_OVERFLOW_ERROR(val, (value<T, Max>()));
		return val;
	}
};

template<typename T, typename Max, typename Min>
struct throw_on_error<T, Max, Min, typename std::enable_if<
	std::is_unsigned<T>::value &&
	(value<T, Min>() == 0)>::type>
{
	T operator() (T val)
	{
		if RANT_UNLIKELY((val > value<T, Max>()))
			throw RANT_OVERFLOW_ERROR(val, (value<T, Max>()));
		return val;
	}
};


template<typename T, typename Max, typename Min,
	typename Enable = void>
struct clip_on_error;

template<typename T, typename Max, typename Min>
struct clip_on_error<T, Max, Min, typename std::enable_if<
	!std::is_unsigned<T>::value ||
	(value<T, Min>() > 0)>::type>
{
	T operator() (T val)
	{
		val = RANT_UNLIKELY((val < value<T, Min>())) ? value<T, Min>() :
			RANT_UNLIKELY((val > value<T, Max>())) ? value<T, Max>() : val;
		return val;
	}
};

template<typename T, typename Max, typename Min>
struct clip_on_error<T, Max, Min, typename std::enable_if<
	std::is_unsigned<T>::value &&
	(value<T, Min>() == 0)>::type>
{
	T operator() (T val)
	{
		return RANT_UNLIKELY((val > value<T, Max>())) ? value<T, Max>() : val;
	}
};

} // namespace rant

#undef RANT_UNDERFLOW_ERROR
#undef RANT_OVERFLOW_ERROR
