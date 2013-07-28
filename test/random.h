// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <type_traits>
#include <random>

template<typename T>
T random()
{
    static std::random_device dev;
    static typename std::conditional<
		std::is_integral<T>::value,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>>::type dist;
	return dist(dev);
}
