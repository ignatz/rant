#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>
#include <string>

#include "rant/util.h"

#define RANT_UNDERFLOW_MESSAGE "underflow error in range type"
#define RANT_OVERFLOW_MESSAGE "overflow error in range type"

namespace rant {

#ifdef RANT_LIGHTWEIGHT_EXCEPTIONS
#define RANT_UNDERFLOW_ERROR(VAL, MIN) std::underflow_error(RANT_UNDERFLOW_MESSAGE)
#define RANT_OVERFLOW_ERROR(VAL, MAX)  std::overflow_error(RANT_OVERFLOW_MESSAGE)
#else
#define RANT_UNDERFLOW_ERROR(VAL, MIN) rant::underflow(VAL, MIN)
#define RANT_OVERFLOW_ERROR(VAL, MAX)  rant::overflow(VAL, MAX)
template<typename T, typename =
	typename std::enable_if<std::is_arithmetic<T>::value>::type>
inline
std::underflow_error
underflow(T val, T min)
{
	std::string s(RANT_UNDERFLOW_MESSAGE ": ");
	s += std::to_string(val) + " < min(";
	s += std::to_string(min) + ")";
	return std::underflow_error(s);
}

template<typename T, typename =
	typename std::enable_if<std::is_arithmetic<T>::value>::type>
inline
std::overflow_error
overflow(T val, T max)
{
	std::string s(RANT_OVERFLOW_MESSAGE ": ");
	s += std::to_string(val) + " > max(";
	s += std::to_string(max) + ")";
	return std::overflow_error(s);
}

template<typename T, typename =
	typename std::enable_if<!std::is_arithmetic<T>::value>::type>
inline
std::underflow_error
underflow(T const&, T const&)
{
	return std::underflow_error("underflow error in range type");
}

template<typename T, typename =
	typename std::enable_if<!std::is_arithmetic<T>::value>::type>
inline
std::overflow_error
overflow(T const&, T const&)
{
	return std::overflow_error("overflow error in range type");
}
#endif // RANT_LIGHTWEIGHT_EXCEPTIONS

template<typename T, typename Max, typename Min>
struct throw_on_error
{
	template<typename ... Args>
	inline
	T operator() (Args&& ... args) const
	{
		T val(std::forward<Args>(args)...);

		if RANT_UNLIKELY(RANT_LESS(val, RANT_VALUE(Min)))
			throw RANT_UNDERFLOW_ERROR(val, RANT_VALUE(Min));

		else if RANT_UNLIKELY(RANT_LESS(RANT_VALUE(Max), val))
			throw RANT_OVERFLOW_ERROR(val, RANT_VALUE(Max));
		return val;
	}
};


template<typename T, typename Max, typename Min>
struct clip_on_error
{
	template<typename ... Args>
	inline
	T operator() (Args&& ... args) const
	{
		T val(std::forward<Args>(args)...);

		return RANT_LESS(val, RANT_VALUE(Min)) ? RANT_VALUE(Min) :
			RANT_LESS(RANT_VALUE(Max), val) ? RANT_VALUE(Max) : val;
	}
};

} // rant

#undef RANT_UNDERFLOW_MESSAGE
#undef RANT_OVERFLOW_MESSAGE
#undef RANT_UNDERFLOW_ERROR
#undef RANT_OVERFLOW_ERROR
