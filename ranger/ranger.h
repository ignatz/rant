#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "ranger/util.h"

#ifndef RANGER_DISABLE
#include "ranger/impl.h"
#else
#pragma message "RANGER_DISABLE is set => no range checking"
namespace ranger {

template<typename T,
	typename Max    = void,
	typename Min    = void,
	typename Enable = void>
using range = T;

} // namespace ranger
#endif // RANGER_DISABLE


// some convinient aliases
namespace ranger {

template<typename T = int,
	T Max = limit<T>::max(),
	T Min = limit<T>::min()>
using integral = range<T, ic<T, Max>, ic<T, Min>, void>;

template<typename T = double,
	typename Max = std::ratio< limit<intmax_t>::max()>,
	typename Min = std::ratio<-limit<intmax_t>::max()>>
using floating_point = range<T, Max, Min>;

} // namespace ranger
