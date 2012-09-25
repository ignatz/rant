#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ranger/util.h"
#include "ranger/check.h"

#define RANGER_EN_IF_RANGE(RET)                         \
	typename std::enable_if<std::is_base_of<            \
			R<T, Max, Min>,                             \
			detail::range_impl<R<T, Max, Min>>          \
		>::value, RET>::type

#define RANGER_OPERATOR_BINARY(OP)                      \
	inline                                              \
	self operator OP (self x) const                     \
	{                                                   \
		return self(this->_val OP x);                   \
	}

#define RANGER_OPERATOR_UNARY(OP)                       \
	inline                                              \
	self operator OP () const                           \
	{                                                   \
		return self(OP this->_val);                     \
	}

#define RANGER_OPERATOR_ASSIGNMENT(OP)                  \
	inline                                              \
	self& operator OP##= (self x)                       \
	{                                                   \
		typedef detail::range_check<T, Max, Min> type;  \
		type::check(this->_val OP x);                   \
		this->_val OP##= x;                             \
		return *static_cast<self*>(this);               \
	}

namespace ranger {

namespace detail {

template<typename Derived>
struct range_impl;

template<template<typename, typename, typename, typename> class Derived,
	typename T, typename Max, typename Min, typename Enable>
struct range_impl<Derived<T, Max, Min, Enable>>
{
	typedef Derived<T, Max, Min, Enable> self;

	range_impl(T val = T()) :
		_val(val)
	{
		range_check<T, Max, Min>::check(val);
	}

	// implicit cast
	inline
	operator T () const noexcept
	{
		return _val;
	}

	// arithmetic operations
	RANGER_OPERATOR_BINARY(+)
	RANGER_OPERATOR_BINARY(-)
	RANGER_OPERATOR_BINARY(*)
	RANGER_OPERATOR_BINARY(/)
	RANGER_OPERATOR_BINARY(%)

	RANGER_OPERATOR_UNARY(+)
	RANGER_OPERATOR_UNARY(-)

	// compound assignment operations
	RANGER_OPERATOR_ASSIGNMENT(+)
	RANGER_OPERATOR_ASSIGNMENT(-)
	RANGER_OPERATOR_ASSIGNMENT(*)
	RANGER_OPERATOR_ASSIGNMENT(/)
	RANGER_OPERATOR_ASSIGNMENT(%)

protected:
	T _val;
};

} // namespace detail



// the actual ranged value class
template<typename T, typename Max, typename Min, typename Enable = void>
struct range;

template<typename T, T Max, T Min>
struct range<T, ic<T, Max>, ic<T, Min>,
		typename std::enable_if<std::is_integral<T>::value>::type
	> :
	public detail::range_impl<range<T, ic<T, Max>, ic<T, Min>>>
{
	typedef range<T, ic<T, Max>, ic<T, Min>> self;

	range(T val = T()) :
		detail::range_impl<range<T, ic<T, Max>, ic<T, Min>>>(val)
	{}

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
		typename std::enable_if<std::is_floating_point<T>::value>::type
	> :
	public detail::range_impl<range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>>>
{
	range(T val = T()) :
		detail::range_impl<range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>>>(val)
	{}
};



template<template<typename, typename, typename> class R,
	typename T, typename Max, typename Min>
RANGER_EN_IF_RANGE(bool)
operator== (R<T, Max, Min> a, R<T, Max, Min> b) noexcept
{
	return static_cast<T>(a) == static_cast<T>(b);
}

template<template<typename, typename, typename> class R,
	typename T, typename Max, typename Min>
RANGER_EN_IF_RANGE(bool)
operator< (R<T, Max, Min> a, R<T, Max, Min> b) noexcept
{
	return static_cast<T>(a) < static_cast<T>(b);
}

template<template<typename, typename, typename> class R,
	typename T, typename Max, typename Min>
RANGER_EN_IF_RANGE(bool)
operator!= (R<T, Max, Min> a, R<T, Max, Min> b) noexcept
{
	return !(a==b);
}

template<template<typename, typename, typename> class R,
	typename T, typename Max, typename Min>
RANGER_EN_IF_RANGE(bool)
operator> (R<T, Max, Min> a, R<T, Max, Min> b) noexcept
{
	return b < a;
}

template<template<typename, typename, typename> class R,
	typename T, typename Max, typename Min>
RANGER_EN_IF_RANGE(bool)
operator<= (R<T, Max, Min> a, R<T, Max, Min> b) noexcept
{
	return !(b<a);
}

template<template<typename, typename, typename> class R,
	typename T, typename Max, typename Min>
RANGER_EN_IF_RANGE(bool)
operator>= (R<T, Max, Min> a, R<T, Max, Min> b) noexcept
{
	return !(a<b);
}

} // namespace ranger
