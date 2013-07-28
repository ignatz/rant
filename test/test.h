// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <gtest/gtest.h>
#include <type_traits>
#include <cstdlib>

#include <rant/rant.h>

#include "test/random.h"

#define REPEAT(N) \
	for (size_t __ii=0; __ii<N; ++__ii)

#define SIGNED_TYPE(N, TYPE) \
	template<> struct signed_type<N> { typedef TYPE type; };

template<size_t N> struct   signed_type;
template<>         struct   signed_type< 8> { typedef  int8_t type; };
template<>         struct   signed_type<16> { typedef int16_t type; };
template<>         struct   signed_type<32> { typedef int32_t type; };
template<>         struct   signed_type<64> { typedef int64_t type; };

template<size_t N> struct unsigned_type;
template<>         struct unsigned_type< 8> { typedef  uint8_t type; };
template<>         struct unsigned_type<16> { typedef uint16_t type; };
template<>         struct unsigned_type<32> { typedef uint32_t type; };
template<>         struct unsigned_type<64> { typedef uint64_t type; };


template<typename T, template<typename> class Functor> struct for_all_types;

template<template <size_t> class T, size_t N, template<typename> class Functor>
struct for_all_types<T<N>, Functor>
{
	auto operator() () const -> decltype(Functor< typename T<N>::type > () ())
	{
		Functor< typename T<N>::type > () ();
		for_all_types<T<N/2>, Functor>() ();
	}
};

template<template <size_t> class T, template<typename> class Functor>
struct for_all_types< T<8>, Functor>
{
	auto operator() () const -> decltype(Functor< typename T<8>::type > () ())
	{
		Functor< typename T<8>::type > () ();
	}
};


template<typename T, bool Expr, T A, T B>
struct cond
{
	enum : T { value = A };
};

template<typename T, T A, T B>
struct cond<T, false, A, B>
{
	enum : T { value = B };
};


template<typename T, T Val>
using ic = std::integral_constant<T, Val>;
