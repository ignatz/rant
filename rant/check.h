#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>
#include <string>
#include <cmath>
#include <boost/utility/enable_if.hpp>
#include <boost/lexical_cast.hpp>

#include "rant/util.h"

#define RANT_CHECK(VAL) Check () (VAL)

namespace rant {

template<typename T, typename Max, typename Min,
	typename Sanitizer, typename = void>
struct SanitizerHelper
{
	template<typename U>
	inline T operator() (U const val) const RANT_NOEXCEPT_COND(
			RANT_NOEXCEPT(Sanitizer::overflow(val)) &&
			RANT_NOEXCEPT(Sanitizer::underflow(val)))
	{
		if RANT_UNLIKELY(RANT_LESS(T, U, val, RANT_VALUE(Min))) {
			return Sanitizer::underflow(val);
		} else if RANT_UNLIKELY(RANT_LESS(T, U, RANT_VALUE(Max), val)) {
			return Sanitizer::overflow(val);
		}
		return val;
	}
};

// specialization for unsingned integers with Min == 0
template<typename T, typename Max, typename Min, typename Sanitizer>
struct SanitizerHelper<T, Max, Min, Sanitizer,
	typename boost::enable_if_c<boost::is_unsigned<T>::type::value &&
		value_helper<T, Min>::value == 0, void>::type>
{
	template<typename U>
	inline T operator() (U const val) const RANT_NOEXCEPT_COND(
			RANT_NOEXCEPT(Sanitizer::overflow(val)) &&
			RANT_NOEXCEPT(Sanitizer::underflow(val)))
	{
		if RANT_UNLIKELY(RANT_LESS(T, U, val, RANT_VALUE(Min))) {
			return Sanitizer::underflow(val);
		} else if RANT_UNLIKELY(RANT_LESS(T, U, RANT_VALUE(Max), val)) {
			return Sanitizer::overflow(val);
		}
		return val;
	}

	// optimized specialization
	inline T operator() (T const& val) const RANT_NOEXCEPT_COND(
			RANT_NOEXCEPT(Sanitizer::overflow(val)))
	{
		if RANT_UNLIKELY(RANT_LESS(T, T, RANT_VALUE(Max), val)) {
			return Sanitizer::overflow(val);
		}
		return val;
	}
};


template<typename T, typename Max, typename Min>
struct throw_on_error :
	public SanitizerHelper<T, Max, Min, throw_on_error<T, Max, Min> >
{
	template<typename U>
	inline static T overflow(U const val)
	{
#ifdef RANT_LIGHTWEIGHT_EXCEPTIONS
		throw std::overflow_error("range overflow");
#else
		std::string s("range overflow: ");
		s += boost::lexical_cast<std::string>(val) + " > max(";
		s += boost::lexical_cast<std::string>(RANT_VALUE(Max)) + ")";
		throw std::overflow_error(s);
#endif
		return T(val);
	}

	template<typename U>
	inline static T underflow(U const val)
	{
#ifdef RANT_LIGHTWEIGHT_EXCEPTIONS
		throw std::underflow_error("range underflow");
#else
		std::string s("range underflow: ");
		s += boost::lexical_cast<std::string>(val) + " < min(";
		s += boost::lexical_cast<std::string>(RANT_VALUE(Min)) + ")";
		throw std::underflow_error(s);
#endif
		return T(val);
	}
};


template<typename T, typename Max, typename Min>
struct clip_on_error :
	public SanitizerHelper<T, Max, Min, clip_on_error<T, Max, Min> >
{
	template<typename U>
	inline static T overflow(U const)
	{
		return RANT_VALUE(Max);
	}

	template<typename U>
	inline static T underflow(U const)
	{
		return RANT_VALUE(Min);
	}
};


template<typename T, typename Max, typename Min, typename = void>
struct wrap_on_error :
	public SanitizerHelper<T, Max, Min, wrap_on_error<T, Max, Min> >
{
	template<typename U>
	inline static T overflow(U const val)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;
		return RANT_VALUE(Min) + ((val-RANT_VALUE(Max)-1)%diff);
	}

	template<typename U>
	inline static T underflow(U const val)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;
		return RANT_VALUE(Max) - ((RANT_VALUE(Min)-val-1)%diff);
	}
};

template<typename T, typename Max, typename Min>
struct wrap_on_error<T, Max, Min,
	typename boost::enable_if_c<boost::is_floating_point<T>::type::value>::type> :
	public SanitizerHelper<T, Max, Min, wrap_on_error<T, Max, Min> >
{
	template<typename U>
	inline static T overflow(U const val)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;
		return RANT_VALUE(Min) + fmod((val-RANT_VALUE(Max)), diff);
	}

	template<typename U>
	inline static T underflow(U const val)
	{
		static const T diff = RANT_VALUE(Max) - RANT_VALUE(Min) + 1;
		return RANT_VALUE(Max) - fmod((RANT_VALUE(Min)-val), diff);
	}
};

} // rant
