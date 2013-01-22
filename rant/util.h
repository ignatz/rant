#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

#ifndef RANT_CONSTEXPR
	#define RANT_CONSTEXPR constexpr
#endif

#ifdef RANT_EXPLICIT_DOWNCAST
	#define RANT_EXPLICIT explicit
#else
	#define RANT_EXPLICIT
#endif // RANT_EXPLICIT_DOWNCAST

#define RANT_VALUE_NAME __val
#define RANT_PACKED __attribute__((packed))

#define RANT_VALUE(VAL) ::rant::value_helper<T, VAL>::get()
#define RANT_LESS(TYPE, LHS, RHS) rant::less(LHS, RHS)

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
	static RANT_CONSTEXPR T get()
		RANT_NOEXCEPT_COND(T())
	{
		return static_cast<T>(Num) / Den;
	}
};


template<typename T, typename U>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
less(T const& t, U const& u)
{
	std::less<long double> r;
	return r(t, u);
}

template<typename T, typename U>
inline
typename std::enable_if<std::is_integral<T>::value, bool>::type
less(T const& t, U const& u)
{
	std::less<intmax_t> r;
	return r(t, u);
}

} // rant
