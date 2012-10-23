#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#ifndef RANT_DISABLE_SERIALIZATION
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#endif // RANT_DISABLE_SERIALIZATION

#include "rant/util.h"
#include "rant/check.h"

#define RANT_CLASS_NAME range
#define RANT_VALUE      __val

#define ADD_QUOTES(STR) #STR
#define STRINGIZE(STR) ADD_QUOTES(STR)

#define RANT_ARITHMETIC_OPS                                        \
	RANT_OPERATOR_BINARY(+)                                        \
	RANT_OPERATOR_BINARY(-)                                        \
	RANT_OPERATOR_BINARY(*)                                        \
	RANT_OPERATOR_BINARY(/)                                        \
	RANT_OPERATOR_UNARY(+)                                         \
	RANT_OPERATOR_UNARY(-)

#ifndef RANT_DISABLE_SERIALIZATION
#define RANT_SERIALIZATION                                         \
private:                                                           \
	friend class boost::serialization::access;                     \
	template<typename Archiver>                                    \
	void serialize(Archiver& ar, unsigned int)                     \
	{                                                              \
		ar & BOOST_SERIALIZATION_NVP(RANT_VALUE);                  \
	}
#else
#define RANT_SERIALIZATION
#endif // RANT_DISABLE_SERIALIZATION

#define RANT_DEFAULT(CLASS_NAME)                                   \
protected:                                                         \
	T RANT_VALUE;                                                  \
public:                                                            \
	typedef CLASS_NAME <T, Max, Min, Check, void> type;            \
                                                                   \
	constexpr CLASS_NAME (T v = T()) : RANT_VALUE(Check(v))        \
	{                                                              \
	}                                                              \
                                                                   \
	inline operator T () const noexcept                            \
	{                                                              \
		return RANT_VALUE;                                         \
	}                                                              \
                                                                   \
	RANT_ARITHMETIC_OPS                                            \

#if __cplusplus != 201103L
#define RANT_STATIC_ASSERT
#else
#define RANT_STATIC_ASSERT                                         \
	static_assert(                                                 \
		value<T, Max>() >= value<T, Min>(), "Max must be >= Min");
#endif

#define RANT_OPERATOR_UNARY_RET(RET, OP)                           \
	inline RET operator OP () const                                \
	{                                                              \
		return RET(OP RANT_VALUE);                                 \
	}

#define RANT_OPERATOR_UNARY(OP)                                    \
	RANT_OPERATOR_UNARY_RET(type, OP)

#define RANT_OPERATOR_ASSIGNMENT(OP)                               \
	inline type& operator OP##= (type x)                           \
	{                                                              \
		RANT_VALUE = Check(RANT_VALUE OP x);                       \
		return *this;                                              \
	}

#define RANT_OPERATOR_BINARY_RET(RET, OP)                          \
	inline RET operator OP (type x) const                          \
	{                                                              \
		return RET(RANT_VALUE OP x);                               \
	}                                                              \

#define RANT_OPERATOR_BINARY(OP)                                   \
	RANT_OPERATOR_BINARY_RET(type, OP)                             \
	RANT_OPERATOR_ASSIGNMENT(OP)                                   \

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

#define RANT_OPERATOR_COMPARE_FF(CLASS_NAME, RET, OP)              \
	template<typename T, typename Max, typename Min>               \
	inline RET operator OP (                                       \
		CLASS_NAME<T, Max, Min> a,                                 \
		CLASS_NAME<T, Max, Min> b) noexcept                        \
	{                                                              \
		return static_cast<T>(a) OP static_cast<T>(b);             \
	}

#define RANT_FWD(CLASS_NAME)                                       \
	template<typename T, typename Max, typename Min,               \
		T(*Check)(T) = throw_on_error<T, Max, Min>,                \
		typename Enable = void>                                    \
	class CLASS_NAME;

#define RANT_IMPL(TYPE, CLASS_NAME, ...)                           \
	template<typename T, typename Max, typename Min, T(*Check)(T)> \
	class CLASS_NAME<T, Max, Min, Check, typename                  \
		std::enable_if<std::is_##TYPE <T>::value>::type>           \
	{                                                              \
		static_assert(std::is_##TYPE <T>::value,                   \
					  "T must be " STRINGIZE(TYPE) " type");       \
		RANT_DEFAULT(CLASS_NAME)                                   \
		RANT_STATIC_ASSERT                                         \
		RANT_SERIALIZATION                                         \
	public:                                                        \
		__VA_ARGS__                                                \
	};


namespace rant {

RANT_FWD(RANT_CLASS_NAME)

RANT_IMPL(floating_point, RANT_CLASS_NAME,
	static_assert(is_ratio<Max>::value,
				  "Max must be std::ratio type");
	static_assert(is_ratio<Min>::value,
				  "Min must be std::ratio type");
)

RANT_IMPL(integral, RANT_CLASS_NAME,
	static_assert(is_integral_constant<Max>::value,
				  "Max must be std::integral_constant type");
	static_assert(is_integral_constant<Min>::value,
				  "Min must be std::integral_constant type");

	RANT_OPERATOR_BINARY(%)
	RANT_OPERATOR_INCREMENTAL(+)
	RANT_OPERATOR_INCREMENTAL(-)

	RANT_OPERATOR_UNARY_RET(bool, !)
	RANT_OPERATOR_BINARY_RET(bool, &&)
	RANT_OPERATOR_BINARY_RET(bool, ||)

	RANT_OPERATOR_UNARY(~)
	RANT_OPERATOR_BINARY(&)
	RANT_OPERATOR_BINARY(|)
	RANT_OPERATOR_BINARY(^)
	RANT_OPERATOR_BINARY(<<)
	RANT_OPERATOR_BINARY(>>)
)

RANT_OPERATOR_COMPARE_FF(RANT_CLASS_NAME, bool, ==)
RANT_OPERATOR_COMPARE_FF(RANT_CLASS_NAME, bool, !=)
RANT_OPERATOR_COMPARE_FF(RANT_CLASS_NAME, bool, <)
RANT_OPERATOR_COMPARE_FF(RANT_CLASS_NAME, bool, >)
RANT_OPERATOR_COMPARE_FF(RANT_CLASS_NAME, bool, <=)
RANT_OPERATOR_COMPARE_FF(RANT_CLASS_NAME, bool, >=)


template<typename T, typename Max, typename Min, T(*Check)(T), typename Enable>
struct numeric_limits<range<T, Max, Min, Check, Enable>>
{
	static constexpr T max() { return value<T, Max>(); }
	static constexpr T lowest() { return value<T, Min>(); }
};

} // namespace rant

#undef RANT_VALUE
#undef ADD_QUOTES
#undef STRINGIZE
#undef RANT_ARITHMETIC_OPS
#undef RANT_SERIALIZATION
#undef RANT_OPERATORS
#undef RANT_DEFAULT
#undef RANT_OPERATOR_UNARY
#undef RANT_OPERATOR_UNARY_RET
#undef RANT_OPERATOR_BINARY
#undef RANT_OPERATOR_BINARY_RET
#undef RANT_OPERATOR_ASSIGNMENT
#undef RANT_OPERATOR_INCREMENTAL
#undef RANT_OPERATOR_COMPARE_FF
#undef RANT_FWD
#undef RANT_IMPL
