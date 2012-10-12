#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"

#ifndef RANT_DISABLE
#include "rant/impl.h"
#else
#pragma message "RANT_DISABLE is set => no range checking"
namespace rant {

template<
	typename T,
	typename Max    = void,
	typename Min    = void,
	T(*Check)(T)    = throw_on_error<T, Max, Min>,
	typename Enable = void>
using range = T;

template<typename T,
	typename Max    = void,
	typename Min    = void,
	T(*Check)(T)    = throw_on_error<T, Max, Min>,
	typename Enable = void>
using clip = T;

} // namespace rant
#endif // RANT_DISABLE


// some convinient aliases
namespace rant {

template<typename T = int,
	T Max = limit<T>::max(),
	T Min = limit<T>::min()>
using irange = range<T, ic<T, Max>, ic<T, Min>>;

template<typename T = double,
	typename Max = std::ratio< limit<intmax_t>::max()>,
	typename Min = std::ratio<-limit<intmax_t>::max()>>
using frange = range<T, Max, Min>;

template<typename T = int,
	T Max = limit<T>::max(),
	T Min = limit<T>::min()>
using iclip = range<T, ic<T, Max>, ic<T, Min>,
	clip_on_error<T, ic<T, Max>, ic<T, Min>>>;

template<typename T = double,
	typename Max = std::ratio< limit<intmax_t>::max()>,
	typename Min = std::ratio<-limit<intmax_t>::max()>>
using fclip = range<T, Max, Min, clip_on_error<T, Max, Min>>;
} // namespace rant
