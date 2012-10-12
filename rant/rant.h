#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"

#include "rant/impl.h"

namespace rant {
namespace opt {

#ifndef RANT_DISABLE
using namespace rant;
#else

#if __cplusplus != 201103L
#error disabling rant requires a newer compiler
#endif

#pragma message "RANT_DISABLE is set"

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
#endif // RANT_DISABLE

} // namespace opt
} // namespace rant


#define RANT_CONVINIENCE_TRAITS                                                   \
template<typename T = int,                                                        \
	T Max = numeric_limits<T>::max(),                                             \
	T Min = numeric_limits<T>::min()>                                             \
struct irange                                                                     \
{                                                                                 \
	typedef range<T,                                                              \
		integral_constant<T, Max>,                                                \
		integral_constant<T, Min>> type;                                          \
};                                                                                \
                                                                                  \
template<typename T = double,                                                     \
	typename Max = std::ratio< numeric_limits<intmax_t>::max()>,                  \
	typename Min = std::ratio<-numeric_limits<intmax_t>::max()>>                  \
struct frange                                                                     \
{                                                                                 \
	typedef range<T, Max, Min> type;                                              \
};                                                                                \
                                                                                  \
template<typename T = int,                                                        \
	T Max = numeric_limits<T>::max(),                                             \
	T Min = numeric_limits<T>::min()>                                             \
struct iclip                                                                      \
{                                                                                 \
	typedef range<T,                                                              \
		integral_constant<T, Max>,                                                \
		integral_constant<T, Min>,                                                \
		clip_on_error<T, integral_constant<T, Max>, integral_constant<T, Min>>    \
	> type;                                                                       \
};                                                                                \
                                                                                  \
template<typename T = double,                                                     \
	typename Max = std::ratio< numeric_limits<intmax_t>::max()>,                  \
	typename Min = std::ratio<-numeric_limits<intmax_t>::max()>>                  \
struct fclip                                                                      \
{                                                                                 \
	typedef range<T, Max, Min, clip_on_error<T, Max, Min>> type;                  \
};


// some convinience aliases
namespace rant {
	RANT_CONVINIENCE_TRAITS
namespace opt {
	RANT_CONVINIENCE_TRAITS
} // namespace opt
} // namespace rant

#undef RANT_CONVINIENCE_TRAITS
