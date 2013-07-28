// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <gtest/gtest.h>
#include <rant/rant.h>

// list of base types
template<typename ... Ts>
struct pack {};

// list of genertators
template<template<typename> class ... Gs>
struct gens {};


template<typename T, typename Pack>
struct prepend;

template<typename ... Ts, typename ... Us>
struct prepend<pack<Ts...>, pack<Us...> >
{
	typedef pack<Ts..., Us...> type;
};


// build a testing matrix from a list of base types and a list of generators.
template<typename Generators, typename Types>
struct cross_product;

template<template<typename> class ... Gs, typename T, typename ... Ts>
struct cross_product< gens<Gs...>, pack<T, Ts...> >
{
	typedef typename prepend<
		pack<typename Gs<T>::type...>,
		typename cross_product< gens<Gs...>, pack<Ts...> >::type
	>::type type;
};

template<template<typename> class ... Gs>
struct cross_product< gens<Gs...>, pack<> >
{
	typedef pack<> type;
};


// fill list of all matrix entries into gtest::Types list.
template<typename Types>
struct test_types;

template<typename ... Ts>
struct test_types< pack<Ts...> >
{
	typedef typename ::testing::Types<Ts...> type;
};



// Thw following types are generator mixins used by the code above to span the
// test matrix over { base types } x { sanitizer }.
template<typename T, T Max, T Min,
	template<typename, typename, typename> class Check>
struct integral_proxy
{
	typedef rant::integral_range<T, Max, Min, Check<T,
			std::integral_constant<T, Max>,
			std::integral_constant<T, Min> > > type;
};

template<typename T, typename Max, typename Min,
	template<typename, typename, typename> class Check>
struct float_proxy;

template<typename T, template<typename, typename, typename> class Check,
	intmax_t MaxN, intmax_t MaxD,
	intmax_t MinN, intmax_t MinD>
struct float_proxy<T, std::ratio<MaxN, MaxD>, std::ratio<MinN, MinD>, Check>
{
	typedef rant::floating_point_range<T,
			std::ratio<MaxN, MaxD>, std::ratio<MinN, MinD>,
			Check<T, std::ratio<MaxN, MaxD>, std::ratio<MinN, MinD> >
		> type;
};
