#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#define BOOST_RATIO_EXTENSIONS
#include <boost/ratio.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer_traits.hpp>
#include <limits>

#define RANT_VALUE_NAME __val
#define RANT_VALUE(VAL) ::rant::value<T, VAL>() ()
#define RANT_PACKED __attribute__((packed))
#define RANT_LESS(LHS, RHS) ::rant::less<T> (LHS, RHS)

#if defined(__GNUC__) && __GNUC__ >= 4
	#define RANT_LIKELY(x) (__builtin_expect((x), 1))
	#define RANT_UNLIKELY(x) (__builtin_expect((x), 0))
#else
	#define RANT_LIKELY(x) (x)
	#define RANT_UNLIKELY(x) (x)
#endif

#define RANT_CONSTEXPR
#define RANT_NOEXCEPT_SPECIFIER
#define RANT_NOEXCEPT_OP
#define RANT_NOEXCEPT_COND(...)
#if __cplusplus >= 201103L
	#define RANT_IS_NOTHROW_DEFAULT_CONSTR(...) \
	RANT_NOEXCEPT_COND( __VA_ARGS__() )
#else
	#define RANT_IS_NOTHROW_DEFAULT_CONSTR(...)
#endif


namespace rant {

template<typename T>
inline
bool less(T const& lhs, T const& rhs)
{
	return std::less<T>() (lhs, rhs);
}


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
struct value;

template<typename T, T Val>
struct value<T, integral_constant<T, Val> > :
	public integral_constant<T, Val>
{
	RANT_CONSTEXPR T operator() () const
		RANT_NOEXCEPT_COND(T())
	{
		return Val;
	}
};

template<typename T, intmax_t Num, intmax_t Den>
struct value<T, boost::ratio<Num, Den> >
{
	RANT_CONSTEXPR T operator() () const
		RANT_NOEXCEPT_COND(T())
	{
		return static_cast<T>(Num) / Den;
	}
};

} // rant
