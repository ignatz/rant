#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/detail/range.h"

#define RANT_RETURN_TYPE_INT                                       \
	typename std::enable_if<std::is_integral<T>::value,            \
		RANT_CLASS_NAME<T, Max, Min, Check>>::type
#define RANT_RETURN_TYPE_INT_BOOL                                  \
	typename std::enable_if<std::is_integral<T>::value, bool>::type

namespace rant {

RANT_IMPL(integral, RANT_CLASS_NAME,
	static_assert(is_integral_constant<Max>::value,
				  "Max must be std::integral_constant type");
	static_assert(is_integral_constant<Min>::value,
				  "Min must be std::integral_constant type");


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
)

RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT, %)

RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT, &)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT, |)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT, ^)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT, <<)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT, >>)

RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT_BOOL, &&)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE_INT_BOOL, ||)

} // namespace rant

#undef RANT_RETURN_TYPE_INT
#undef RANT_RETURN_TYPE_INT_BOOL
