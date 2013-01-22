#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
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
private:
	static_assert(std::is_floating_point<T>::value,
				  "T must be floating point type");
	static_assert(is_ratio<Max>::value,
				  "Max must be std::ratio type");
	static_assert(is_ratio<Min>::value,
				  "Min must be std::ratio type");
	static_assert(std::ratio_less_equal<Min, Max>::value,
				  "Max must be >= Min");

	typedef floating_point_range<T, Max, Min, Check> type;
	typedef T value_type;

public:
	template<typename U = T>
	RANT_CONSTEXPR floating_point_range(U v = T())
			RANT_NOEXCEPT_COND(RANT_CHECK(v)) :
		RANT_VALUE_NAME(RANT_CHECK(v)) {}

	inline
	RANT_CONSTEXPR RANT_EXPLICIT operator T () const
		RANT_NOEXCEPT_SPECIFIER
	{
		return RANT_VALUE_NAME;
	}

	RANT_OP_ASSIGNMENT(+)
	RANT_OP_ASSIGNMENT(-)
	RANT_OP_ASSIGNMENT(*)
	RANT_OP_ASSIGNMENT(/)
	RANT_OP_UNARY(+, type)
	RANT_OP_UNARY(-, type)

protected:
	T RANT_VALUE_NAME;
} RANT_PACKED;

#define RANT_FLOAT_RET floating_point_range<T, Max, Min, Check>

RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, ==, bool)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, !=, bool)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename,  <, bool)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename,  >, bool)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, <=, bool)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, >=, bool)

RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, +, RANT_FLOAT_RET)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, -, RANT_FLOAT_RET)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, *, RANT_FLOAT_RET)
RANT_OP_BINARY_FF_BUILTIN(floating_point_range, typename, /, RANT_FLOAT_RET)

} // rant

RANT_OP_STREAM(rant::floating_point_range, typename)


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
