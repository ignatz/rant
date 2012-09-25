#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <stdexcept>

#include "ranger/util.h"

namespace ranger {
namespace detail {

template<typename T, typename Max, typename Min, typename Enable = void>
struct range_check;

template<typename T, typename Max, typename Min>
struct range_check<T, Max, Min, typename std::enable_if<std::is_integral<T>::value>::type>
{
	static_assert(Max::value >= Min::value, "Max must be >= Min");

	static void check(T val)
	{
		if (!(val >= Min::value))
			throw std::underflow_error("underflow");
		else if (!(val < Max::value))
			throw std::overflow_error("overflow");
	}
};

template<typename T, typename Max, typename Min>
struct range_check<T, Max, Min, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	static_assert(std::ratio_greater_equal<Max, Min>::value,
				  "Max must be >= Min");

	static void check(T val)
	{
		if (!(val >= static_cast<T>(Min::num) / Min::den))
			throw std::underflow_error("underflow");
		else if (!(val < static_cast<T>(Max::num) / Max::den))
			throw std::overflow_error("overflow");
	}
};

} // namespace detail
} // namespace ranger