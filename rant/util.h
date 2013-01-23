#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>
#include <iostream>

#ifndef RANT_CONSTEXPR
	#define RANT_CONSTEXPR constexpr
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

#define RANT_NOEXCEPT_SPECIFIER noexcept
#define RANT_NOEXCEPT_OP noexcept
#define RANT_NOEXCEPT_COND(...) \
	RANT_NOEXCEPT_SPECIFIER(RANT_NOEXCEPT_OP(__VA_ARGS__))
#if __cplusplus >= 201103L
	#define RANT_IS_NOTHROW_DEFAULT_CONSTR(...) \
	RANT_NOEXCEPT_COND( __VA_ARGS__() )
#else
	#define RANT_IS_NOTHROW_DEFAULT_CONSTR(...)
#endif


namespace rant {

template<typename T>
struct is_ratio :
	public std::integral_constant<bool, false> {};

template<intmax_t Num, intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> :
	public std::integral_constant<bool, true> {};


template<typename T, typename Val>
struct value_helper;

template<typename T, T Val>
struct value_helper<T, std::integral_constant<T, Val>> :
	public std::integral_constant<T, Val>
{
	typedef T type;

	static RANT_CONSTEXPR T get()
		RANT_NOEXCEPT_COND(T())
	{
		return Val;
	}
};

template<typename T, intmax_t Num, intmax_t Den>
struct value_helper<T, std::ratio<Num, Den>> :
	public std::integral_constant<intmax_t, Num>
{
	typedef T type;

	static RANT_CONSTEXPR T get()
		RANT_NOEXCEPT_COND(T())
	{
		return static_cast<T>(Num) / Den;
	}
};


template<typename T>
struct is_unsigned_integral :
	public std::is_unsigned<T>
{};

template<typename T>
struct is_signed_integral :
	public std::integral_constant<bool,
		std::is_integral<T>::value && std::is_signed<T>::value>
{};


template<typename T, typename U>
struct max
{
	typedef typename std::conditional<
		(sizeof(T) > sizeof(U)), T, U>::type type;
};


template<typename T, typename U, typename = void>
struct promote;

template<typename T, typename U>
struct promote<T, U, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	typedef typename std::conditional<
		std::is_floating_point<U>::value,
		typename max<T, U>::type,
		T
	>::type type;
};

template<typename T, typename U>
struct promote<T, U, typename std::enable_if<is_signed_integral<T>::value>::type>
{
	typedef typename std::conditional<
		is_signed_integral<U>::value,
		typename max<T, U>::type,
		intmax_t
	>::type type;
};

template<typename T, typename U>
struct promote<T, U, typename std::enable_if<is_unsigned_integral<T>::value>::type>
{
	typedef typename std::conditional<
		is_unsigned_integral<U>::value,
		typename max<T, U>::type,
		typename std::conditional<
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
