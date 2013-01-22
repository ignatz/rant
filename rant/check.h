#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>
#include <string>
#include <cmath>

#include "rant/util.h"

#define RANT_CHECK(VAL) Check () (VAL)

#define RANT_UNDERFLOW_MESSAGE "range underflow"
#define RANT_OVERFLOW_MESSAGE "range overflow"

namespace rant {

#ifdef RANT_LIGHTWEIGHT_EXCEPTIONS
#define RANT_UNDERFLOW_ERROR(VAL, MIN) std::underflow_error(RANT_UNDERFLOW_MESSAGE)
#define RANT_OVERFLOW_ERROR(VAL, MAX)  std::overflow_error(RANT_OVERFLOW_MESSAGE)
#else
#define RANT_UNDERFLOW_ERROR(VAL, MIN) rant::underflow(VAL, MIN)
#define RANT_OVERFLOW_ERROR(VAL, MAX)  rant::overflow(VAL, MAX)
template<typename T, typename U>
std::underflow_error
underflow(T const val, U const min)
{
	std::string s(RANT_UNDERFLOW_MESSAGE ": ");
	s += std::to_string(val) + " < min(";
	s += std::to_string(min) + ")";
	return std::underflow_error(s);
}

template<typename T, typename U>
std::overflow_error
overflow(T const val, U const max)
{
	std::string s(RANT_OVERFLOW_MESSAGE ": ");
	s += std::to_string(val) + " > max(";
	s += std::to_string(max) + ")";
	return std::overflow_error(s);
}
#endif // RANT_LIGHTWEIGHT_EXCEPTIONS


template<typename T, typename Max, typename Min>
struct throw_on_error
{
	template<typename U>
	typename std::enable_if<
		!std::is_unsigned<U>::value ||
		value_helper<T, Min>::value != 0, T>::type
	operator() (U const val) const
	{
		if RANT_UNLIKELY(RANT_LESS(T, val, RANT_VALUE(Min))) {
			throw RANT_UNDERFLOW_ERROR(val, RANT_VALUE(Min));
		} else if RANT_UNLIKELY(RANT_LESS(T, RANT_VALUE(Max), val)) {
			throw RANT_OVERFLOW_ERROR(val, RANT_VALUE(Max));
		}
		return val;
	}

	template<typename U>
	typename std::enable_if<
		std::is_unsigned<U>::value &&
		value_helper<T, Min>::value == 0, T>::type
	operator() (U const val) const
	{
		if RANT_UNLIKELY(RANT_LESS(T, RANT_VALUE(Max), val)) {
			throw RANT_OVERFLOW_ERROR(val, RANT_VALUE(Max));
		}
		return val;
	}
};


template<typename T, typename Max, typename Min>
struct clip_on_error
{
	template<typename U>
	typename std::enable_if<
		!std::is_unsigned<U>::value ||
		value_helper<T, Min>::value != 0, T>::type
	operator() (U const val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		return RANT_LESS(T, val, RANT_VALUE(Min)) ? RANT_VALUE(Min) :
			RANT_LESS(T, RANT_VALUE(Max), val) ? RANT_VALUE(Max) : val;
	}

	template<typename U>
	typename std::enable_if<
		std::is_unsigned<U>::value &&
		value_helper<T, Min>::value == 0, T>::type
	operator() (U const val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		return RANT_LESS(T, RANT_VALUE(Max), val) ? RANT_VALUE(Max) : val;
	}
};


template<typename T, typename Max, typename Min, typename = void>
struct wrap_on_error
{
	template<typename U>
	T operator() (U const val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;

		if (RANT_LESS(T, val, RANT_VALUE(Min))) {
			return RANT_VALUE(Max) - ((RANT_VALUE(Min)-val-1)%diff);
		} else if (RANT_LESS(T, RANT_VALUE(Max), val)) {
			return RANT_VALUE(Min) + ((val-RANT_VALUE(Max)-1)%diff);
		}
		return val;
	}
};

template<typename T, typename Max, typename Min>
struct wrap_on_error<T, Max, Min,
	typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	template<typename U>
	T operator() (U const val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;

		if (RANT_LESS(T, val, RANT_VALUE(Min))) {
			return RANT_VALUE(Max) - fmod((RANT_VALUE(Min)-val), diff);
		} else if (RANT_LESS(T, RANT_VALUE(Max), val)) {
			return RANT_VALUE(Min) + fmod((val-RANT_VALUE(Max)), diff);
		}
		return val;
	}
};

} // rant

#undef RANT_UNDERFLOW_MESSAGE
#undef RANT_OVERFLOW_MESSAGE
#undef RANT_UNDERFLOW_ERROR
#undef RANT_OVERFLOW_ERROR
