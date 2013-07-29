// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include <typeinfo>
#include "test/test.h"
#include "test/types.h"

using namespace rant;

TEST(Util, SameKind)
{
	ASSERT_TRUE((same_kind<char, int>::value));
	ASSERT_TRUE((same_kind<uintmax_t, unsigned short>::value));

	ASSERT_FALSE((same_kind<unsigned, int>::value));
	ASSERT_FALSE((same_kind<short, uintmax_t>::value));
}


template<typename T, typename U>
struct Inner
{
	void operator() () const
	{
		REPEAT(10000)
		{
			T const t = random<T>();
			U const u = random<U>();

			ASSERT_EQ((mpint_t(t) < mpint_t(u)), (RANT_LESS(T, U, t, u)))
				<< typeid(T).name() << " " << typeid(U).name();
		}
	}
};

template<typename T>
struct Outer
{
	void operator() () const
	{
		for_all_types<  signed_type<64>, Inner, T> () ();
		for_all_types<unsigned_type<64>, Inner, T> () ();
	}
};

TEST(Util, Less)
{
	for_all_types<  signed_type<64>, Outer> () ();
	for_all_types<unsigned_type<64>, Outer> () ();
}
