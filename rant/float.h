#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"
#include "rant/op.h"

namespace rant {

template<typename T,
	typename Max   = std::ratio< std::numeric_limits<std::intmax_t>::max()>,
	typename Min   = std::ratio<-std::numeric_limits<std::intmax_t>::max()>,
	typename Check = throw_on_error<T, Max, Min>>
class floating_point_range
{
	static_assert(std::is_floating_point<T>::value,
				  "T must be floating point type");
	static_assert(is_ratio<Max>::value,
				  "Max must be std::ratio type");
	static_assert(is_ratio<Min>::value,
				  "Min must be std::ratio type");
	static_assert(std::ratio_less_equal<Min, Max>::value,
				  "Max must be >= Min");

public:
	typedef floating_point_range<T, Max, Min, Check> type;
	typedef T value_type;

	RANT_CONSTEXPR floating_point_range(T v = T()) :
		RANT_VALUE_NAME(RANT_CHECK(v)) {}

	inline
	RANT_EXPLICIT operator T () const noexcept
	{
		return RANT_VALUE_NAME;
	}

	RANT_OP_ASSIGNMENT(+)
	RANT_OP_ASSIGNMENT(-)
	RANT_OP_ASSIGNMENT(*)
	RANT_OP_ASSIGNMENT(/)
	RANT_OP_UNARY(type, +)
	RANT_OP_UNARY(type, -)

protected:
	T RANT_VALUE_NAME;
} __attribute__((packed));

#define RANT_FLOAT_RET floating_point_range<T, Max, Min, Check>

RANT_OP_BINARY_FF_BUILTIN(floating_point_range, bool, typename, ==)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, bool, typename, !=)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, bool, typename, <)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, bool, typename, >)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, bool, typename, <=)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, bool, typename, >=)

RANT_OP_BINARY_FF_BUILTIN(floating_point_range, RANT_FLOAT_RET, typename, +)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, RANT_FLOAT_RET, typename, -)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, RANT_FLOAT_RET, typename, *)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, RANT_FLOAT_RET, typename, /)

} // rant


namespace std {

template<typename T, typename Max,
	typename Min, typename Check>
struct numeric_limits<rant::floating_point_range<T, Max, Min, Check>> :
	public std::numeric_limits<T>
{
	static RANT_CONSTEXPR T max() { return RANT_VALUE(Max); }
	static RANT_CONSTEXPR T lowest() { return RANT_VALUE(Min); }
};

} // std

RANT_OP_STREAM(rant::floating_point_range, typename)
