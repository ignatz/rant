#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/range.h"

#define RANT_OPERATOR_INCREMENTAL(OP)                         \
	inline type& operator OP##OP ()                           \
	{                                                         \
		return *this OP##= 1;                                 \
	}                                                         \
	inline type operator OP##OP (int)                         \
	{                                                         \
		type t(*this);                                        \
		*this OP##= 1;                                        \
		return t;                                             \
	}

namespace rant {

template<typename T,
	T Max   = std::numeric_limits<T>::max(),
	T Min   = std::numeric_limits<T>::min(),
	typename Check = throw_on_error<T,
		std::integral_constant<T, Max>,
		std::integral_constant<T, Min>>>
class integral_range
{
	static_assert(std::is_integral<T>::value,
				  "T must be integral type");
	static_assert(Min <= Max, "Max must be >= Min");

public:
	typedef integral_range<T, Max, Min, Check> type;
	typedef T value_type;

	RANT_CONSTEXPR integral_range(T v = T()) :
		RANT_VALUE_NAME(Check() (v)) {}

	inline
	RANT_EXPLICIT operator T () const noexcept
	{
		return RANT_VALUE_NAME;
	}

	RANT_OPERATOR_ASSIGNMENT(+)
	RANT_OPERATOR_ASSIGNMENT(-)
	RANT_OPERATOR_ASSIGNMENT(*)
	RANT_OPERATOR_ASSIGNMENT(/)
	RANT_OPERATOR_UNARY_RET(type, +)
	RANT_OPERATOR_UNARY_RET(type, -)


	RANT_OPERATOR_ASSIGNMENT(%)
	RANT_OPERATOR_INCREMENTAL(+)
	RANT_OPERATOR_INCREMENTAL(-)

	RANT_OPERATOR_UNARY_RET(bool, !)
	RANT_OPERATOR_UNARY_RET(type, ~)

	RANT_OPERATOR_ASSIGNMENT(&)
	RANT_OPERATOR_ASSIGNMENT(|)
	RANT_OPERATOR_ASSIGNMENT(^)
	RANT_OPERATOR_ASSIGNMENT(<<)
	RANT_OPERATOR_ASSIGNMENT(>>)

protected:
	T RANT_VALUE_NAME;
} __attribute__((packed));

RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, ==)
RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, !=)
RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, <)
RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, >)
RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, <=)
RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, >=)

RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, +)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, -)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, *)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, /)


RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, %)

RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, &)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, |)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, ^)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, <<)
RANT_OPERATOR_BINARY_FF_RET(integral_range, T, T, >>)

RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, &&)
RANT_OPERATOR_BINARY_FF_RET(integral_range, bool, T, ||)

} // rant


namespace std {

template<typename T, T Max,
	T Min, typename Check>
struct numeric_limits<rant::integral_range<T, Max, Min, Check>> :
	public std::numeric_limits<T>
{
	static RANT_CONSTEXPR T max() { return Max; }
	static RANT_CONSTEXPR T lowest() { return Min; }
};

} // std

#undef RANT_OPERATOR_INCREMENTAL
