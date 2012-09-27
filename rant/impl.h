#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#ifndef RANT_DISABLE_SERIALIZATION
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#endif // RANT_DISABLE_SERIALIZATION

#include "rant/util.h"
#include "rant/check.h"

#define RANT_VALUE __val

#define RANT_ARITHMETIC_OPS                                   \
	RANT_OPERATOR_BINARY(self, +)                             \
	RANT_OPERATOR_BINARY(self, -)                             \
	RANT_OPERATOR_BINARY(self, *)                             \
	RANT_OPERATOR_BINARY(self, /)                             \
	RANT_OPERATOR_UNARY(self, +)                              \
	RANT_OPERATOR_UNARY(self, -)

#define RANT_ASSIGNMENT_OPS                                   \
	RANT_OPERATOR_ASSIGNMENT(+)                               \
	RANT_OPERATOR_ASSIGNMENT(-)                               \
	RANT_OPERATOR_ASSIGNMENT(*)                               \
	RANT_OPERATOR_ASSIGNMENT(/)

#ifndef RANT_DISABLE_SERIALIZATION
#define RANT_SERIALIZATION                                    \
private:                                                      \
	friend class boost::serialization::access;                \
	template<typename Archiver>                               \
	void serialize(Archiver& ar, unsigned int)                \
	{                                                         \
		ar & BOOST_SERIALIZATION_NVP(RANT_VALUE);             \
	}
#else
#define RANT_SERIALIZATION
#endif // RANT_DISABLE_SERIALIZATION

#define RANT_OPERATORS(...)                                   \
protected:                                                    \
	T RANT_VALUE;                                             \
public:                                                       \
	typedef range<T, Max, Min> self;                          \
                                                              \
	range(T v = T()) : RANT_VALUE(v)                          \
	{                                                         \
		check<T, Max, Min>(RANT_VALUE);                       \
	}                                                         \
                                                              \
	inline operator T () const noexcept                       \
	{                                                         \
		return RANT_VALUE;                                    \
	}                                                         \
                                                              \
	static constexpr T min() noexcept                         \
	{                                                         \
		return value<T, Min>();                               \
	}                                                         \
                                                              \
	static constexpr T max() noexcept                         \
	{                                                         \
		return value<T, Max>();                               \
	}                                                         \
	static_assert(max() >= min(), "Max must be >= Min");      \
                                                              \
	RANT_ARITHMETIC_OPS                                       \
	RANT_ASSIGNMENT_OPS                                       \
	__VA_ARGS__

#define RANT_OPERATOR_UNARY(RET, OP)                          \
	inline RET operator OP () const                           \
	{                                                         \
		return RET(OP RANT_VALUE);                            \
	}

#define RANT_OPERATOR_BINARY(RET, OP)                         \
	inline RET operator OP (self x) const                     \
	{                                                         \
		return RET(RANT_VALUE OP x);                          \
	}

#define RANT_OPERATOR_ASSIGNMENT(OP)                          \
	inline self& operator OP##= (self x)                      \
	{                                                         \
		check<T, Max, Min>(RANT_VALUE OP x);                  \
		RANT_VALUE OP##= x;                                   \
		return *this;                                         \
	}

#define RANT_OPERATOR_INCREMENTAL(OP)                         \
	inline self& operator OP##OP ()                           \
	{                                                         \
		return *this OP##= 1;                                 \
	}                                                         \
	inline self operator OP##OP (int)                         \
	{                                                         \
		self t(*this);                                        \
		*this OP##= 1;                                        \
		return t;                                             \
	}

#define RANT_OPERATOR_COMPARE_FF(RET, OP)                     \
	template<typename T, typename Max, typename Min>          \
	inline RET operator OP (                                  \
		range<T, Max, Min> a,                                 \
		range<T, Max, Min> b) noexcept                        \
	{                                                         \
		return static_cast<T>(a) OP static_cast<T>(b);        \
	}


namespace rant {

// the actual ranged value class
template<typename T, typename Max, typename Min, typename Enable = void>
class range;

template<typename T, typename Max, typename Min>
class range<T, Max, Min,
	typename std::enable_if<std::is_integral<T>::value>::type>
{
	static_assert(std::is_integral<T>::value,
				  "T must be integral type");
	static_assert(is_integral_constant<Max>::value,
				  "Max must be std::integral_constant type");
	static_assert(is_integral_constant<Min>::value,
				  "Min must be std::integral_constant type");

	RANT_OPERATORS
	(
		// integral arithmetic operations
		RANT_OPERATOR_BINARY(self, %)
		RANT_OPERATOR_ASSIGNMENT(%)
		RANT_OPERATOR_INCREMENTAL(+)
		RANT_OPERATOR_INCREMENTAL(-)

		// integral logical operations
		RANT_OPERATOR_UNARY(bool, !)
		RANT_OPERATOR_BINARY(bool, &&)
		RANT_OPERATOR_BINARY(bool, ||)

		// integral bitwise operations
		RANT_OPERATOR_UNARY(self, ~)
		RANT_OPERATOR_BINARY(self, &)
		RANT_OPERATOR_BINARY(self, |)
		RANT_OPERATOR_BINARY(self, ^)
		RANT_OPERATOR_BINARY(self, <<)
		RANT_OPERATOR_BINARY(self, >>)
	)
	RANT_SERIALIZATION
};


template<typename T, typename Max, typename Min>
class range<T, Max, Min,
	typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	static_assert(std::is_floating_point<T>::value,
				  "T must be floating point type");
	static_assert(is_ratio<Max>::value,
				  "Max must be std::ratio type");
	static_assert(is_ratio<Min>::value,
				  "Min must be std::ratio type");

	RANT_OPERATORS()
	RANT_SERIALIZATION
};


// comparison operators
RANT_OPERATOR_COMPARE_FF(bool, ==)
RANT_OPERATOR_COMPARE_FF(bool, !=)
RANT_OPERATOR_COMPARE_FF(bool, <)
RANT_OPERATOR_COMPARE_FF(bool, >)
RANT_OPERATOR_COMPARE_FF(bool, <=)
RANT_OPERATOR_COMPARE_FF(bool, >=)

} // namespace rant

#undef RANT_ARITHMETIC_OPS
#undef RANT_ASSIGNMENT_OPS
#undef RANT_SERIALIZATION
#undef RANT_OPERATORS
#undef RANT_OPERATOR_BINARY
#undef RANT_OPERATOR_UNARY
#undef RANT_OPERATOR_ASSIGNMENT
#undef RANT_OPERATOR_COMPARE_FF
