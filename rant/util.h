#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#define BOOST_RATIO_EXTENSIONS
#include <boost/ratio.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer_traits.hpp>
#include <limits>

#ifndef RANT_CONSTEXPR
	#define RANT_CONSTEXPR
#endif

#define RANT_VALUE_NAME __val
#define RANT_PACKED __attribute__((packed))

#define RANT_VALUE(VAL) ::rant::value_helper<T, VAL>::get()
#define RANT_LESS(TYPE, U, LHS, RHS) rant::less<TYPE, U>::compare(LHS, RHS)

#if defined(__GNUC__) && __GNUC__ >= 4
	#define RANT_LIKELY(x) (__builtin_expect((x), 1))
	#define RANT_UNLIKELY(x) (__builtin_expect((x), 0))
#else
	#define RANT_LIKELY(x) (x)
	#define RANT_UNLIKELY(x) (x)
#endif

// macros for noexcept specifier and operator
#define RANT_NOEXCEPT_SPECIFIER
#define RANT_NOEXCEPT
#if __cplusplus >= 201103L
	#define RANT_NOEXCEPT_COND(...) \
		RANT_NOEXCEPT_SPECIFIER(__VA_ARGS__)
	#define RANT_IS_NOTHROW_DEFAULT_CONSTR(...) \
		RANT_NOEXCEPT_COND(RANT_NOEXCEPT( __VA_ARGS__() ))
#else
	#define RANT_NOEXCEPT_COND(...)
	#define RANT_IS_NOTHROW_DEFAULT_CONSTR(...)
#endif


namespace rant {

template<typename T, T Val>
struct integral_constant
{
	static const T value = Val;
};


template<typename T>
struct is_ratio :
	public integral_constant<bool, false> {};

template<intmax_t Num, intmax_t Den>
struct is_ratio<boost::ratio<Num, Den> > :
	public integral_constant<bool, true> {};


template<typename T, typename Val>
struct value_helper;

template<typename T, T Val>
struct value_helper<T, integral_constant<T, Val> > :
	public integral_constant<T, Val>
{
	typedef T type;

	static RANT_CONSTEXPR T get()
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		return Val;
	}
};

template<typename T, intmax_t Num, intmax_t Den>
struct value_helper<T, boost::ratio<Num, Den> > :
	public integral_constant<intmax_t, Num>
{
	typedef T type;

	static RANT_CONSTEXPR T get()
		RANT_IS_NOTHROW_DEFAULT_CONSTR(T)
	{
		return static_cast<T>(Num) / Den;
	}
};


template<typename T>
struct is_unsigned_integral :
	public boost::is_unsigned<T>::type
{};

template<typename T>
struct is_signed_integral :
	public integral_constant<bool,
		boost::is_integral<T>::type::value && boost::is_signed<T>::type::value>
{};


template<typename T, typename U>
struct max
{
	typedef typename boost::conditional<
		(sizeof(T) > sizeof(U)), T, U>::type type;
};


template<typename T, typename U, typename = void>
struct promote;

template<typename T, typename U>
struct promote<T, U, typename boost::enable_if_c<boost::is_floating_point<T>::type::value>::type>
{
	typedef typename boost::conditional<
		boost::is_floating_point<U>::type::value,
		typename max<T, U>::type,
		T
	>::type type;
};

template<typename T, typename U>
struct promote<T, U, typename boost::enable_if_c<is_signed_integral<T>::value>::type>
{
	typedef typename boost::conditional<
		is_signed_integral<U>::value,
		typename max<T, U>::type,
		intmax_t
	>::type type;
};

template<typename T, typename U>
struct promote<T, U, typename boost::enable_if_c<is_unsigned_integral<T>::value>::type>
{
	typedef typename boost::conditional<
		is_unsigned_integral<U>::value,
		typename max<T, U>::type,
		typename boost::conditional<
			(sizeof(U) < sizeof(intmax_t)),
			intmax_t,
			uintmax_t
		>::type
	>::type type;
};


template<typename T, typename U>
struct less
{
	inline
	static bool compare(
		 typename promote<T, U>::type const t,
		 typename promote<T, U>::type const u)
	{
		return t<u;
	}
};

} // rant
