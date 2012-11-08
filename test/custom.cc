#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <type_traits>

#include "rant/rant.h"

using namespace rant;

struct A
{
	constexpr A(int i=0) : i(i) {}
	int i;
	bool operator<(A const& other) const { return i < other.i; }
};

struct Upper
{
	A const& operator() () const
	{
		static A a{42};
		return a;
	}
};

struct Lower
{
	A const& operator() () const
	{
		static A a{5};
		return a;
	}
};


TEST(Basic, Custom)
{
	typedef range<A, Upper, Lower> type;

	type i(A(20));

	ASSERT_THROW(type (),     std::underflow_error);
	ASSERT_THROW(type (A(4)), std::underflow_error);
	ASSERT_THROW(type (A(100)), std::overflow_error);
}
