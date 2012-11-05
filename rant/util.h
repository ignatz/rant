#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

#if defined(__GNUC__) && __GNUC__ >= 4
#define RANT_LIKELY(x) (__builtin_expect((x), 1))
#define RANT_UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define RANT_LIKELY(x) (x)
#define RANT_UNLIKELY(x) (x)
#endif

namespace rant {

using std::integral_constant;
using std::intmax_t;


template<typename T>
struct is_integral_constant :
	public integral_constant<bool, false> {};

template<typename T, T Val>
struct is_integral_constant<std::integral_constant<T, Val>> :
	public integral_constant<bool, true> {};


template<typename T>
struct is_ratio :
	public integral_constant<bool, false> {};

template<intmax_t Num, intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> :
	public integral_constant<bool, true> {};


template<typename T, typename Val>
constexpr
typename std::enable_if<std::is_integral<T>::value, T>::type
value()
{
	static_assert(is_integral_constant<Val>::value,
				  "limit must be of type integral_constant");
	return Val::value;
}

template<typename T, typename Val>
constexpr
typename std::enable_if<std::is_floating_point<T>::value, T>::type
value()
{
	static_assert(is_ratio<Val>::value, "limit must be of type ratio");
	return static_cast<T>(Val::num) / Val::den;
}


template<typename T>
struct numeric_limits
{
	static constexpr T max()    { return std::numeric_limits<T>::max(); }
	static constexpr T lowest() { return std::numeric_limits<T>::lowest(); }
};

} // namespace rant
