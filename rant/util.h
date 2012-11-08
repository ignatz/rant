#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

#define RANT_CONSTEXPR constexpr

#ifdef __clang__
#define RANT_VALUE(VAL) ::rant::value<T, VAL>() ()
#define RANT_LESS(LHS, RHS) ::std::less<T>() (LHS, RHS)
#else
#define RANT_VALUE(VAL) ::rant::value<T, VAL>{} ()
#define RANT_LESS(LHS, RHS) ::std::less<T>{} (LHS, RHS)
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#define RANT_LIKELY(x) (__builtin_expect((x), 1))
#define RANT_UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define RANT_LIKELY(x) (x)
#define RANT_UNLIKELY(x) (x)
#endif

namespace rant {

template<typename T>
struct is_ratio :
	public std::integral_constant<bool, false> {};

template<intmax_t Num, intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> :
	public std::integral_constant<bool, true> {};


template<typename T, typename Val>
struct value
{
	T const& operator() () const
	{
		return static_cast<T const&>(Val() ());
	}
};

template<typename T, T Val>
struct value<T, std::integral_constant<T, Val>>
{
	RANT_CONSTEXPR T operator() () const
	{
		return Val;
	}
};

template<typename T, intmax_t Num, intmax_t Den>
struct value<T, std::ratio<Num, Den>>
{
	RANT_CONSTEXPR T operator() () const
	{
		return static_cast<T>(Num) / Den;
	}
};

} // rant
