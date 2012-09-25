#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"

#ifndef RANT_DISABLE
#include "rant/impl.h"
#else
#pragma message "RANT_DISABLE is set => no range checking"
namespace rant {

template<typename T,
	typename Max    = void,
	typename Min    = void,
	typename Enable = void>
using range = T;

} // namespace rant
#endif // RANT_DISABLE


// some convinient aliases
namespace rant {

template<typename T = int,
	T Max = limit<T>::max(),
	T Min = limit<T>::min()>
using integral = range<T, ic<T, Max>, ic<T, Min>, void>;

template<typename T = double,
	typename Max = std::ratio< limit<intmax_t>::max()>,
	typename Min = std::ratio<-limit<intmax_t>::max()>>
using floating_point = range<T, Max, Min>;

} // namespace rant
