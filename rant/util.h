#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

namespace rant {

template<typename T, T Val>
using ic = std::integral_constant<T, Val>;

template<typename T>
struct is_integral_constant :
	public ic<bool, false> {};

template<typename T, T Val>
struct is_integral_constant<std::integral_constant<T, Val>> :
	public ic<bool, true> {};


using std::intmax_t;

template<typename T>
struct is_ratio :
	public ic<bool, false> {};

template<intmax_t Num, intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> :
	public ic<bool, true> {};


template<typename T>
using limit = std::numeric_limits<T>;


template<typename T, typename Val>
constexpr
typename std::enable_if<std::is_integral<T>::value, T>::type
value()
{
	return Val::value;
}

template<typename T, typename Val>
constexpr
typename std::enable_if<std::is_floating_point<T>::value, T>::type
value()
{
	return static_cast<T>(Val::num) / Val::den;
}

} // namespace rant
