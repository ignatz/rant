#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"

#define RANT_CLASS_NAME range
#define RANT_VALUE      __val

#ifdef RANT_EXPLICIT_DOWNCAST
#define RANT_EXPLICIT explicit
#else
#define RANT_EXPLICIT
#endif // RANT_EXPLICIT_DOWNCAST

#define RANT_ADD_QUOTES(STR) #STR
#define RANT_STRINGIZE(STR) RANT_ADD_QUOTES(STR)

#define RANT_OPERATOR_UNARY_RET(RET, OP)                           \
	inline RET operator OP () const                                \
	{                                                              \
		return OP RANT_VALUE;                                      \
	}

#define RANT_OPERATOR_ASSIGNMENT(OP)                               \
	inline type& operator OP##= (T x)                              \
	{                                                              \
		RANT_VALUE = Check() (RANT_VALUE OP x);                    \
		return *this;                                              \
	}                                                              \
	inline type& operator OP##= (type x)                           \
	{                                                              \
		return operator OP##=(static_cast<T>(x));                  \
	}

#define RANT_OPERATOR_INCREMENTAL(OP)                              \
	inline type& operator OP##OP ()                                \
	{                                                              \
		return *this OP##= 1;                                      \
	}                                                              \
	inline type operator OP##OP (int)                              \
	{                                                              \
		type t(*this);                                             \
		*this OP##= 1;                                             \
		return t;                                                  \
	}

#define RANT_OPERATOR_BINARY_FF_RET(CLASS_NAME, RET, OP)           \
	template<typename T, typename Max,                             \
		typename Min, typename Check>                              \
	inline RET operator OP (                                       \
		CLASS_NAME<T, Max, Min, Check> a,                          \
		CLASS_NAME<T, Max, Min, Check> b) noexcept                 \
	{                                                              \
		return static_cast<T>(a) OP static_cast<T>(b);             \
	}                                                              \
	template<typename U, typename T, typename Max,                 \
		typename Min, typename Check>                              \
	inline typename                                                \
	std::enable_if<std::is_arithmetic<U>::value, RET>::type        \
	operator OP (                                                  \
		CLASS_NAME<T, Max, Min, Check> a,                          \
		U b) noexcept                                              \
	{                                                              \
		return static_cast<T>(a) OP b;                             \
	}                                                              \
	template<typename U, typename T, typename Max,                 \
		typename Min, typename Check>                              \
	inline typename                                                \
	std::enable_if<std::is_arithmetic<U>::value, RET>::type        \
	operator OP (                                                  \
		U a,                                                       \
		CLASS_NAME<T, Max, Min, Check> b) noexcept                 \
	{                                                              \
		return a OP static_cast<T>(b);                             \
	}

#define RANT_FWD(CLASS_NAME)                                       \
	template<typename T, typename Max, typename Min,               \
		typename Check = throw_on_error<T, Max, Min>,              \
		typename Enable = void>                                    \
	class CLASS_NAME;

#if __cplusplus != 201103L
#define RANT_STATIC_ASSERT
#else
#define RANT_STATIC_ASSERT                                         \
	static_assert(value<T, Max>() >= value<T, Min>(),              \
				  "Max must be >= Min");
#endif

#define RANT_DEFAULT_BODY(CLASS_NAME)                              \
protected:                                                         \
	T RANT_VALUE;                                                  \
public:                                                            \
	typedef CLASS_NAME <T, Max, Min, Check, void> type;            \
	typedef T                                     value_type;      \
                                                                   \
	constexpr CLASS_NAME (T v = T()) : RANT_VALUE(Check() (v)) {}  \
                                                                   \
	inline RANT_EXPLICIT operator T () const noexcept              \
	{                                                              \
		return RANT_VALUE;                                         \
	}                                                              \
                                                                   \
	RANT_OPERATOR_ASSIGNMENT(+)                                    \
	RANT_OPERATOR_ASSIGNMENT(-)                                    \
	RANT_OPERATOR_ASSIGNMENT(*)                                    \
	RANT_OPERATOR_ASSIGNMENT(/)                                    \
	RANT_OPERATOR_UNARY_RET(type, +)                               \
	RANT_OPERATOR_UNARY_RET(type, -)

#define RANT_IMPL(TYPE, CLASS_NAME, ...)                           \
	template<typename T, typename Max,                             \
		typename Min, typename Check>                              \
	class CLASS_NAME<T, Max, Min, Check, typename                  \
		std::enable_if<std::is_##TYPE <T>::value>::type>           \
	{                                                              \
		static_assert(std::is_##TYPE <T>::value,                   \
					  "T must be " RANT_STRINGIZE(TYPE) " type");  \
		RANT_DEFAULT_BODY(CLASS_NAME)                              \
		RANT_STATIC_ASSERT                                         \
	public:                                                        \
		__VA_ARGS__                                                \
	};

#define RANT_RETURN_TYPE RANT_CLASS_NAME<T, Max, Min, Check>

namespace rant {

RANT_FWD(RANT_CLASS_NAME)

template<typename T, typename Max,
	typename Min, typename Check, typename Enable>
struct numeric_limits<range<T, Max, Min, Check, Enable>>
{
	static constexpr T max() { return value<T, Max>(); }
	static constexpr T lowest() { return value<T, Min>(); }
};

RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE, +)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE, -)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE, *)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, RANT_RETURN_TYPE, /)

RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, bool, ==)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, bool, !=)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, bool, <)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, bool, >)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, bool, <=)
RANT_OPERATOR_BINARY_FF_RET(RANT_CLASS_NAME, bool, >=)

} // namespace rant

#include "rant/detail/int.h"
#include "rant/detail/float.h"

#undef RANT_VALUE
#undef RANT_ADD_QUOTES
#undef RANT_STRINGIZE
#undef RANT_DEFAULT_BODY
#undef RANT_STATIC_ASSERT
#undef RANT_OPERATOR_UNARY_RET
#undef RANT_OPERATOR_ASSIGNMENT
#undef RANT_OPERATOR_INCREMENTAL
#undef RANT_OPERATOR_BINARY_FF_RET
#undef RANT_FWD
#undef RANT_IMPL
#undef RANT_RETURN_TYPE
