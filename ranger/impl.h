#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#ifndef __WITHOUT_BOOST__
#include <boost/serialization/serialization.hpp>
#endif // __WITHOUT_BOOST__

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

#define RANGER_SERIALIZATION                                  \
	friend class boost::serialization::access;                \
	template<typename Archiver>                               \
	void serialize(Archiver& ar, unsigned int)                \
	{                                                         \
		ar & _val;                                            \
	}

#define RANGER_BASE                                           \
protected:                                                    \
	T _val;                                                   \
public:                                                       \
	typedef range<T, Max, Min> self;                          \
                                                              \
	range(T val = T()) : _val(val)                            \
	{                                                         \
		check<T, Max, Min>(val);                              \
	}                                                         \
                                                              \
	inline operator T () const noexcept                       \
	{                                                         \
		return _val;                                          \
	}                                                         \

#define RANGER_OPERATOR_BINARY(OP)                            \
	inline                                                    \
	self operator OP (self x) const                           \
	{                                                         \
		return self(this->_val OP x);                         \
	}

#define RANGER_OPERATOR_UNARY(OP)                             \
	inline                                                    \
	self operator OP () const                                 \
	{                                                         \
		return self(OP this->_val);                           \
	}

#define RANGER_OPERATOR_ASSIGNMENT(OP)                        \
	inline                                                    \
	self& operator OP##= (self x)                             \
	{                                                         \
		check<T, Max, Min>(this->_val OP x);                  \
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
class range;

template<typename T, typename Max, typename Min>
class range<T, Max, Min,
		typename std::enable_if<std::is_integral<T>::value>::type>
{
private:
	static_assert(std::is_integral<T>::value,
				  "T must be integral type");
	static_assert(is_integral_constant<Max>::value,
				  "Max must be std::integral_constant type");
	static_assert(is_integral_constant<Min>::value,
				  "Min must be std::integral_constant type");

public:
	RANGER_BASE
	RANGER_ARITHMETIC_OPS
	RANGER_ASSIGNMENT_OPS

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

#ifndef __WITHOUT_BOOST__
private:
	RANGER_SERIALIZATION
#endif // __WITHOUT_BOOST__
};


template<typename T, typename Max, typename Min>
class range<T, Max, Min,
		typename std::enable_if<std::is_floating_point<T>::value>::type>
{
private:
	static_assert(std::is_floating_point<T>::value,
				  "T must be floating point type");
	static_assert(is_ratio<Max>::value,
				  "Max must be std::ratio type");
	static_assert(is_ratio<Min>::value,
				  "Min must be std::ratio type");

public:
	RANGER_BASE
	RANGER_ARITHMETIC_OPS
	RANGER_ASSIGNMENT_OPS

#ifndef __WITHOUT_BOOST__
private:
	RANGER_SERIALIZATION
#endif // __WITHOUT_BOOST__
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
