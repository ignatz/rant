#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"
#include "rant/op.h"

#define RANT_OP_INCREMENTAL(OP) \
	inline type& operator OP##OP () \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(type) \
	{ \
		return *this OP##= 1; \
	} \
	inline type operator OP##OP (int) \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(type) \
	{ \
		type t(*this); \
		*this OP##= 1; \
		return t; \
	}

namespace rant {
template<typename T,
	std::intmax_t Max = std::numeric_limits<T>::max(),
	std::intmax_t Min = std::numeric_limits<T>::min(),
	typename Check = throw_on_error<T,
		std::integral_constant<T, Max>,
		std::integral_constant<T, Min>>>
class integral_range
{
private:
	static_assert(std::is_integral<T>::value,
				  "T must be integral type");
	static_assert(Min <= Max, "Max must be >= Min");
	static_assert(Min >= std::numeric_limits<T>::lowest(),
				  "Min smaller than minimum value of T");
	static_assert(Max <= std::numeric_limits<T>::max(),
				  "Max bigger than maximum value of T");

	typedef integral_range<T, Max, Min, Check> type;
	typedef T value_type;

public:
	RANT_CONSTEXPR integral_range(T v = T())
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


	RANT_OP_ASSIGNMENT(%)
	RANT_OP_INCREMENTAL(+)
	RANT_OP_INCREMENTAL(-)

	RANT_OP_UNARY(!, bool)
	RANT_OP_UNARY(~, type)

	RANT_OP_ASSIGNMENT(&)
	RANT_OP_ASSIGNMENT(|)
	RANT_OP_ASSIGNMENT(^)
	RANT_OP_ASSIGNMENT(<<)
	RANT_OP_ASSIGNMENT(>>)

protected:
	T RANT_VALUE_NAME;
} RANT_PACKED;

#define RANT_INT_RET integral_range<T, Max, Min, Check>

RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, ==, bool)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, !=, bool)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t,  <, bool)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t,  >, bool)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, <=, bool)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, >=, bool)

RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, +, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, -, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, *, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, /, RANT_INT_RET)


RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, %, RANT_INT_RET)

RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t,  &, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t,  |, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t,  ^, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, <<, RANT_INT_RET)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, >>, RANT_INT_RET)

RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, &&, bool)
RANT_OP_BINARY_FF_BUILTIN(integral_range, std::intmax_t, ||, bool)

} // rant

RANT_OP_STREAM(rant::integral_range, std::intmax_t)


namespace std {

template<typename T, std::intmax_t Max,
	std::intmax_t Min, typename Check>
struct numeric_limits<rant::integral_range<T, Max, Min, Check>> :
	public std::numeric_limits<T>
{
	static RANT_CONSTEXPR T max() { return Max; }
	static RANT_CONSTEXPR T lowest() { return Min; }
};

} // std

#undef RANT_OP_INCREMENTAL
