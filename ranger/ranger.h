#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

#ifndef RANGER_DISABLE
	#include "ranger/check.h"
	#define RANGER_CHECK(VAL) \
		detail::range_check<T, Max, Min>::check(VAL)
#else
	#pragma message "RANGER_DISABLE is set => no range checking"
	#define RANGER_CHECK(VAL)
#endif // RANGER_DISABLE

#define RANGER_EN_IF_RANGE(RET)                \
	typename std::enable_if<std::is_base_of<   \
			R<T, Max, Min>,                    \
			detail::range_impl<R<T, Max, Min>> \
		>::value, RET>::type

namespace ranger {

// some type imports
using std::intmax_t;

template<typename T, T Val>
using ic = std::integral_constant<T, Val>;

template<bool Expr, typename T = void>
using eif = std::enable_if<Expr, T>;

template<intmax_t Num, intmax_t Den = 1>
using ratio = std::ratio<Num, Den>;


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
		RANGER_CHECK(_val);
	}

	// implicit cast
	inline
	operator T () const noexcept
	{
		return _val;
	}


	// arithmetic operations
	inline
	self operator+ (self x) const
	{
		return self(_val + x);
	}

	inline
	self operator- (self x) const
	{
		return self(_val - x);
	}

	inline
	self operator* (self x) const
	{
		return self(_val * x);
	}

	inline
	self operator/ (self x) const
	{
		return self(_val / x);
	}

	// compound assignment operations
	inline
	self& operator+= (self x)
	{
		RANGER_CHECK(_val+x);
		_val += x;
		return *static_cast<self*>(this);
	}

	inline
	self& operator-= (self x)
	{
		RANGER_CHECK(_val-x);
		_val -= x;
		return *static_cast<self*>(this);
	}

	inline
	self& operator*= (self x)
	{
		RANGER_CHECK(_val*x);
		_val *= x;
		return *static_cast<self*>(this);
	}

	inline
	self& operator/= (self x)
	{
		RANGER_CHECK(_val/x);
		_val /= x;
		return *static_cast<self*>(this);
	}

protected:
	T _val;
};

} // namespace detail



// the actual ranged value class
template<typename T, typename Max, typename Min, typename Enable = void>
struct range;

template<typename T, T Max, T Min>
struct range<T, ic<T, Max>, ic<T, Min>,
		typename eif<std::is_integral<T>::value>::type
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
};

template<typename T,
	intmax_t MaxNum, intmax_t MaxDen,
	intmax_t MinNum, intmax_t MinDen>
struct range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>,
		typename eif<std::is_floating_point<T>::value>::type
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



// some convinient aliases
template<typename T = int,
	T Max = std::numeric_limits<T>::max(),
	T Min = std::numeric_limits<T>::min()>
using integral = range<T, ic<T, Max>, ic<T, Min>, void>;

template<typename T = double,
	typename Max = ratio<std::numeric_limits<intmax_t>::max()>,
	typename Min = ratio<std::numeric_limits<intmax_t>::min()>>
using floating_point = range<T, Max, Min>;

} // namespace ranger
