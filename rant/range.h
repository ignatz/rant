#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"

#define RANT_VALUE_NAME_NAME __val

#ifdef RANT_EXPLICIT_DOWNCAST
#define RANT_EXPLICIT explicit
#else
#define RANT_EXPLICIT
#endif // RANT_EXPLICIT_DOWNCAST

#define RANT_OPERATOR_UNARY_RET(RET, OP)                        \
	inline RET operator OP () const                             \
	{                                                           \
		return OP RANT_VALUE_NAME;                              \
	}

#define RANT_OPERATOR_ASSIGNMENT(OP)                            \
	inline type& operator OP##= (T x)                           \
	{                                                           \
		RANT_VALUE_NAME = Check() (RANT_VALUE_NAME OP x);       \
		return *this;                                           \
	}                                                           \
	inline type& operator OP##= (type x)                        \
	{                                                           \
		return operator OP##=(static_cast<T>(x));               \
	}

#define RANT_OPERATOR_BINARY_FF_RET(CLASS_NAME, RET, ARG, OP)   \
	template<typename T, ARG Max,                               \
		ARG Min, typename Check>                                \
	inline RET operator OP (                                    \
		CLASS_NAME<T, Max, Min, Check> a,                       \
		CLASS_NAME<T, Max, Min, Check> b) noexcept              \
	{                                                           \
		return static_cast<T>(a) OP static_cast<T>(b);          \
	}                                                           \
	template<typename U, typename T, ARG Max,                   \
		ARG Min, typename Check>                                \
	inline typename                                             \
	std::enable_if<std::is_arithmetic<U>::value, RET>::type     \
	operator OP (                                               \
		CLASS_NAME<T, Max, Min, Check> a,                       \
		U b) noexcept                                           \
	{                                                           \
		return static_cast<T>(a) OP b;                          \
	}                                                           \
	template<typename U, typename T, ARG Max,                   \
		ARG Min, typename Check>                                \
	inline typename                                             \
	std::enable_if<std::is_arithmetic<U>::value, RET>::type     \
	operator OP (                                               \
		U a,                                                    \
		CLASS_NAME<T, Max, Min, Check> b) noexcept              \
	{                                                           \
		return a OP static_cast<T>(b);                          \
	}


namespace rant {

template<typename T, typename Max, typename Min,
	typename Check = throw_on_error<T, Max, Min>>
struct range : public T
{
	inline
	RANT_EXPLICIT operator T& ()
	{
		return static_cast<T&>(*this);
	}

	inline
	RANT_EXPLICIT operator T const& () const
	{
		return static_cast<T const&>(*this);
	}

	template<typename ... Args>
	range(Args&& ... args) :
		T(Check() (std::forward<Args>(args)...)) {}
} __attribute__((packed));

RANT_OPERATOR_BINARY_FF_RET(range, bool, typename, <)

} // rant
