#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

#ifdef NDEBUG
#pragma message "NDEBUG is set; no range checking"
#define RANGER_CHECK(VAL)
#define RANGER_CONSTEXPR constexpr
#else
#include "ranger/check.h"
#define RANGER_CHECK(VAL) _check<T, Max, Min>::check(VAL)
#define RANGER_CONSTEXPR
#endif // NDEBUG

#define RANGER_EN_IF_RANGE(RET)                \
	typename std::enable_if<std::is_base_of<   \
			R<T, Max, Min>,                    \
			detail::_impl<                     \
				R<T, Max, Min>                 \
			>                                  \
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
struct _impl;

template<template<typename, typename, typename, typename> class Derived,
	typename T, typename Max, typename Min, typename Enable>
struct _impl<Derived<T, Max, Min, Enable>>
{
	typedef Derived<T, Max, Min, Enable> self;

	RANGER_CONSTEXPR
	_impl(T val = T()) :
		_val(val)
	{
		RANGER_CHECK(_val);
	}

	inline
	operator T () const noexcept
	{
		return _val;
	}

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

	inline
	void operator+= (self x)
	{
		RANGER_CHECK(_val+x);
		_val += x;
	}

	inline
	void operator-= (self x)
	{
		RANGER_CHECK(_val-x);
		_val -= x;
	}

	// prefix ++/-- operators
	self& operator++ ()
	{
		_val += 1;
		return *static_cast<self*>(this);
	}

	self& operator-- ()
	{
		_val -= 1;
		return *static_cast<self*>(this);
	}

	// postfix ++/-- operators
	self operator++ (int)
	{
		self t(*this);
		self(_val + 1);
		return t;
	}

	self operator-- (int)
	{
		self t(*this);
		self(_val - 1);
		return t;
	}

private:
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
	public detail::_impl<range<T, ic<T, Max>, ic<T, Min>>>
{
	RANGER_CONSTEXPR
	range(T val = T()) :
		detail::_impl<range<T, ic<T, Max>, ic<T, Min>>>(val)
	{}
};

template<typename T,
	intmax_t MaxNum, intmax_t MaxDen,
	intmax_t MinNum, intmax_t MinDen>
struct range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>,
		typename eif<std::is_floating_point<T>::value>::type
	> :
	public detail::_impl<range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>>>
{
	RANGER_CONSTEXPR
	range(T val = T()) :
		detail::_impl<range<T, ratio<MaxNum, MaxDen>, ratio<MinNum, MinDen>>>(val)
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
