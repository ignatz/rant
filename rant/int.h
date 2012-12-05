#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"
#include "rant/op.h"

#define RANT_OP_INCREMENTAL(OP)                               \
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
private:
	static_assert(std::is_integral<T>::value,
				  "T must be integral type");
	static_assert(Min <= Max, "Max must be >= Min");

	typedef integral_range<T, Max, Min, Check> type;
	typedef T value_type;

public:
	RANT_CONSTEXPR integral_range(T v = T()) :
		RANT_VALUE_NAME(RANT_CHECK(v)) {}

	inline
	RANT_CONSTEXPR RANT_EXPLICIT operator T () const noexcept
	{
		return RANT_VALUE_NAME;
	}

	RANT_OP_ASSIGNMENT(+)
	RANT_OP_ASSIGNMENT(-)
	RANT_OP_ASSIGNMENT(*)
	RANT_OP_ASSIGNMENT(/)
	RANT_OP_UNARY(type, +)
	RANT_OP_UNARY(type, -)


	RANT_OP_ASSIGNMENT(%)
	RANT_OP_INCREMENTAL(+)
	RANT_OP_INCREMENTAL(-)

	RANT_OP_UNARY(bool, !)
	RANT_OP_UNARY(type, ~)

	RANT_OP_ASSIGNMENT(&)
	RANT_OP_ASSIGNMENT(|)
	RANT_OP_ASSIGNMENT(^)
	RANT_OP_ASSIGNMENT(<<)
	RANT_OP_ASSIGNMENT(>>)

protected:
	T RANT_VALUE_NAME;
} __attribute__((packed));

#define RANT_INT_RET integral_range<T, Max, Min, Check>

RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, ==)
RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, !=)
RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, <)
RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, >)
RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, <=)
RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, >=)

RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, +)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, -)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, *)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, /)


RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, %)

RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, &)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, |)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, ^)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, <<)
RANT_OP_BINARY_FF_BUILTIN(integral_range, RANT_INT_RET, T, >>)

RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, &&)
RANT_OP_BINARY_FF_BUILTIN(integral_range, bool, T, ||)

} // rant

RANT_OP_STREAM(rant::integral_range, T)

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

#undef RANT_OP_INCREMENTAL
