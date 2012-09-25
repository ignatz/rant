#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#ifndef __WITHOUT_BOOST__
#include <boost/serialization/serialization.hpp>
#endif // __WITHOUT_BOOST__

#include "rant/util.h"
#include "rant/check.h"

#define RANT_ARITHMETIC_OPS                                   \
	RANT_OPERATOR_BINARY(+)                                   \
	RANT_OPERATOR_BINARY(-)                                   \
	RANT_OPERATOR_BINARY(*)                                   \
	RANT_OPERATOR_BINARY(/)                                   \
	RANT_OPERATOR_BINARY(%)                                   \
	RANT_OPERATOR_UNARY(+)                                    \
	RANT_OPERATOR_UNARY(-)

#define RANT_ASSIGNMENT_OPS                                   \
	RANT_OPERATOR_ASSIGNMENT(+)                               \
	RANT_OPERATOR_ASSIGNMENT(-)                               \
	RANT_OPERATOR_ASSIGNMENT(*)                               \
	RANT_OPERATOR_ASSIGNMENT(/)                               \
	RANT_OPERATOR_ASSIGNMENT(%)                               \

#define RANT_SERIALIZATION                                    \
	friend class boost::serialization::access;                \
	template<typename Archiver>                               \
	void serialize(Archiver& ar, unsigned int)                \
	{                                                         \
		ar & _val;                                            \
	}

#define RANT_BASE                                             \
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

#define RANT_OPERATOR_BINARY(OP)                              \
	inline                                                    \
	self operator OP (self x) const                           \
	{                                                         \
		return self(this->_val OP x);                         \
	}

#define RANT_OPERATOR_UNARY(OP)                               \
	inline                                                    \
	self operator OP () const                                 \
	{                                                         \
		return self(OP this->_val);                           \
	}

#define RANT_OPERATOR_ASSIGNMENT(OP)                          \
	inline                                                    \
	self& operator OP##= (self x)                             \
	{                                                         \
		check<T, Max, Min>(this->_val OP x);                  \
		this->_val OP##= x;                                   \
		return *static_cast<self*>(this);                     \
	}

#define RANT_OPERATOR_COMPARE_FF(RET, OP)                     \
	template<typename T, typename Max, typename Min>          \
	inline                                                    \
	RET operator OP (                                         \
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
private:
	static_assert(std::is_integral<T>::value,
				  "T must be integral type");
	static_assert(is_integral_constant<Max>::value,
				  "Max must be std::integral_constant type");
	static_assert(is_integral_constant<Min>::value,
				  "Min must be std::integral_constant type");

public:
	RANT_BASE
	RANT_ARITHMETIC_OPS
	RANT_ASSIGNMENT_OPS

	// logical operations
	RANT_OPERATOR_UNARY(!)
	RANT_OPERATOR_BINARY(&&)
	RANT_OPERATOR_BINARY(||)

	// bitwise operations
	RANT_OPERATOR_UNARY(~)
	RANT_OPERATOR_BINARY(&)
	RANT_OPERATOR_BINARY(|)
	RANT_OPERATOR_BINARY(^)
	RANT_OPERATOR_BINARY(<<)
	RANT_OPERATOR_BINARY(>>)

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
	RANT_SERIALIZATION
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
	RANT_BASE
	RANT_ARITHMETIC_OPS
	RANT_ASSIGNMENT_OPS

#ifndef __WITHOUT_BOOST__
private:
	RANT_SERIALIZATION
#endif // __WITHOUT_BOOST__
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
#undef RANT_BASE
#undef RANT_OPERATOR_BINARY
#undef RANT_OPERATOR_UNARY
#undef RANT_OPERATOR_ASSIGNMENT
#undef RANT_OPERATOR_COMPARE_FF
