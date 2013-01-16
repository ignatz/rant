#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <rant/int.h>
#include <rant/float.h>

namespace rant {

template<typename T>
struct traits
{
	static_assert(std::is_integral<T>::value,
				  "arithmetic or range type expected");
	typedef T type;

	enum : T {
			   min = std::numeric_limits<T>::min(),
			   max = std::numeric_limits<T>::max()
		   };
};

template<typename T, T Max, T Min, typename Check>
struct traits<integral_range<T, Max, Min, Check>>
{

	typedef T type;
	typedef Check check_type;

	enum : T {
			   min = Min,
			   max = Max
		   };
};

template<typename T, typename Max, typename Min, typename Check>
struct traits<floating_point_range<T, Max, Min, Check>>
{
	typedef T type;
	typedef Check check_type;
	typedef Min min;
	typedef Max max;
};

} // rant
