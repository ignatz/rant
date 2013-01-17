#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
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
};

template<typename T, T Max, T Min, typename Check>
struct traits<integral_range<T, Max, Min, Check>>
{

	typedef T type;
	typedef Check callback_type;
};

template<typename T, typename Max, typename Min, typename Check>
struct traits<floating_point_range<T, Max, Min, Check>>
{
	typedef T type;
	typedef Check callback_type;
};

} // rant
