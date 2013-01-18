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
template<typename T>
typename std::enable_if<
	std::is_arithmetic<T>::value,
	std::underflow_error>::type
underflow(T val, T min)
{
	std::string s(RANT_UNDERFLOW_MESSAGE ": ");
	s += std::to_string(val) + " < min(";
	s += std::to_string(min) + ")";
	return std::underflow_error(s);
}

template<typename T>
typename std::enable_if<
	std::is_arithmetic<T>::value,
	std::overflow_error>::type
overflow(T val, T max)
{
	std::string s(RANT_OVERFLOW_MESSAGE ": ");
	s += std::to_string(val) + " > max(";
	s += std::to_string(max) + ")";
	return std::overflow_error(s);
}
#endif // RANT_LIGHTWEIGHT_EXCEPTIONS


template<typename T, typename Max, typename Min, typename = void>
struct throw_on_error
{
	inline
	T operator() (T val) const
	{
		if RANT_UNLIKELY(RANT_LESS(val, RANT_VALUE(Min)))
			throw RANT_UNDERFLOW_ERROR(val, RANT_VALUE(Min));

		else if RANT_UNLIKELY(RANT_LESS(RANT_VALUE(Max), val))
			throw RANT_OVERFLOW_ERROR(val, RANT_VALUE(Max));
		return val;
	}
};

template<typename T, typename Max, typename Min>
struct throw_on_error<T, Max, Min,
	typename std::enable_if<std::is_unsigned<T>::value && !Min::value>::type>
{
	inline
	T operator() (T val) const
	{
		if RANT_UNLIKELY(RANT_LESS(RANT_VALUE(Max), val))
			throw RANT_OVERFLOW_ERROR(val, RANT_VALUE(Max));
		return val;
	}
};


template<typename T, typename Max, typename Min, typename = void>
struct clip_on_error
{
	inline
	T operator() (T val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		return RANT_LESS(val, RANT_VALUE(Min)) ? RANT_VALUE(Min) :
			RANT_LESS(RANT_VALUE(Max), val) ? RANT_VALUE(Max) : val;
	}
};

template<typename T, typename Max, typename Min>
struct clip_on_error<T, Max, Min,
	typename std::enable_if<std::is_unsigned<T>::value && !Min::value>::type>
{
	inline
	T operator() (T val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		return RANT_LESS(RANT_VALUE(Max), val) ? RANT_VALUE(Max) : val;
	}
};


template<typename T, typename Max, typename Min, typename = void>
struct wrap_on_error
{
	//static_assert(std::is_integral<T>::value,
				  //"wrapping only supports integer types so far");
	inline
	T operator() (T val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;

		if (RANT_LESS(val, RANT_VALUE(Min))) {
			return RANT_VALUE(Max) - ((RANT_VALUE(Min)-val-1)%diff);
		} else if (RANT_LESS(RANT_VALUE(Max), val)) {
			return RANT_VALUE(Min) + ((val-RANT_VALUE(Max)-1)%diff);
		}
		return val;
	}
};

template<typename T, typename Max, typename Min>
struct wrap_on_error<T, Max, Min,
	typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	inline
	T operator() (T val) const
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;

		if (RANT_LESS(val, RANT_VALUE(Min))) {
			return RANT_VALUE(Max) - fmod((RANT_VALUE(Min)-val), diff);
		} else if (RANT_LESS(RANT_VALUE(Max), val)) {
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
