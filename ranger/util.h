#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <limits>
#include <type_traits>
#include <ratio>

namespace ranger {

// some type imports
using std::intmax_t;

template<typename T, T Val>
using ic = std::integral_constant<T, Val>;

template<intmax_t Num, intmax_t Den = 1>
using ratio = std::ratio<Num, Den>;

template<typename T>
using limit = std::numeric_limits<T>;

} // namespace ranger
