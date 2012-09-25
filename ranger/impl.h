#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ranger/util.h"
#include "ranger/check.h"

#define RANGER_ARITHMETIC_OPS                                 \
	RANGER_OPERATOR_BINARY(+)                                 \
	RANGER_OPERATOR_BINARY(-)                                 \
	RANGER_OPERATOR_BINARY(*)                                 \
	RANGER_OPERATOR_BINARY(/)                                 \
	RANGER_OPERATOR_BINARY(%)                                 \
	RANGER_OPERATOR_UNARY(+)                                  \
	RANGER_OPERATOR_UNARY(-)

#define RANGER_ASSIGNMENT_OPS                                 \
	RANGER_OPERATOR_ASSIGNMENT(+)                             \
	RANGER_OPERATOR_ASSIGNMENT(-)                             \
	RANGER_OPERATOR_ASSIGNMENT(*)                             \
	RANGER_OPERATOR_ASSIGNMENT(/)                             \
	RANGER_OPERATOR_ASSIGNMENT(%)                             \

#define RANGER_BASE                                           \
protected:                                                    \
	T _val;                                                   \
public:                                                       \
	typedef range<T, max, min> self;                          \
                                                              \
	range(T val = T()) : _val(val)                            \
	{                                                         \
		check<T, max, min>(val);                              \
	}                                                         \
                                                              \
	inline operator T () const noexcept                       \
	{                                                         \
		return _val;                                          \
	}                                                         \

#define RANGER_OPERATOR_BINARY(OP)                            \
public:                                                       \
	inline                                                    \
	self operator OP (self x) const                           \
	{                                                         \
		return self(this->_val OP x);                         \
	}

#define RANGER_OPERATOR_UNARY(OP)                             \
public:                                                       \
	inline                                                    \
	self operator OP () const                                 \
	{                                                         \
		return self(OP this->_val);                           \
	}

#define RANGER_OPERATOR_ASSIGNMENT(OP)                        \
public:                                                       \
	inline                                                    \
	self& operator OP##= (self x)                             \
	{                                                         \
		check<T, max, min>(this->_val OP x);                  \
		this->_val OP##= x;                                   \
		return *static_cast<self*>(this);                     \
	}

#define RANGER_OPERATOR_COMPARE_FF(RET, OP)                   \
	template<typename T, typename Max, typename Min>          \
	inline                                                    \
	RET operator OP (                                         \
		range<T, Max, Min> a,                                 \
		range<T, Max, Min> b) noexcept                        \
	{                                                         \
		return static_cast<T>(a) OP static_cast<T>(b);        \
	}


namespace ranger {

// the actual ranged value class
template<typename T, typename Max, typename Min, typename Enable = void>
struct range;

template<typename T, T Max, T Min>
struct range<T, ic<T, Max>, ic<T, Min>,
		typename std::enable_if<std::is_integral<T>::value>::type>
{
	static_assert(std::is_integral<T>::value,
		"T must be integral type");
	typedef ic<T, Max> max;
	typedef ic<T, Min> min;

	RANGER_BASE
	RANGER_ARITHMETIC_OPS
	RANGER_ASSIGNMENT_OPS

	// prefix ++/-- operators
	self& operator++ ()
	{
		return *this += 1;
	}

	self& operator-- ()
	{
		return *this -= 1;
	}

	// postfix ++/-- operators
	self operator++ (int)
	{
		self t(*this);
		*this += 1;
		return t;
	}

	self operator-- (int)
	{
		self t(*this);
		*this -= 1;
		return t;
	}

	// logical operations
	RANGER_OPERATOR_UNARY(!)

	RANGER_OPERATOR_BINARY(&&)
	RANGER_OPERATOR_BINARY(||)

	// bitwise operations
	RANGER_OPERATOR_UNARY(~)

	RANGER_OPERATOR_BINARY(&)
	RANGER_OPERATOR_BINARY(|)
	RANGER_OPERATOR_BINARY(^)
	RANGER_OPERATOR_BINARY(<<)
	RANGER_OPERATOR_BINARY(>>)
};


template<typename T,
	intmax_t MaxNum, intmax_t MaxDen,
	intmax_t MinNum, intmax_t MinDen>
struct range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>,
		typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	static_assert(std::is_floating_point<T>::value,
		"T must be floating point type");
	typedef ratio<MaxNum, MaxDen> max;
	typedef ratio<MinNum, MinDen> min;

	RANGER_BASE
	RANGER_ARITHMETIC_OPS
	RANGER_ASSIGNMENT_OPS
};


// comparison operators
RANGER_OPERATOR_COMPARE_FF(bool, ==)
RANGER_OPERATOR_COMPARE_FF(bool, !=)
RANGER_OPERATOR_COMPARE_FF(bool, <)
RANGER_OPERATOR_COMPARE_FF(bool, >)
RANGER_OPERATOR_COMPARE_FF(bool, <=)
RANGER_OPERATOR_COMPARE_FF(bool, >=)

} // namespace ranger

#undef RANGER_ARITHMETIC_OPS
#undef RANGER_ASSIGNMENT_OPS
#undef RANGER_BASE
#undef RANGER_OPERATOR_BINARY
#undef RANGER_OPERATOR_UNARY
#undef RANGER_OPERATOR_ASSIGNMENT
#undef RANGER_OPERATOR_COMPARE_FF
