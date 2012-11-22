#include <gtest/gtest.h>

#define RANT_EXPLICIT

#include "rant/rant.h"

using namespace rant;


template<typename T>
void test_implicit_cast()
{
	T t(42);
	typename T::value_type b = t;
	static_cast<void>(b);
}

TEST(ImplicitCast, Base)
{
	typedef integral_range<int> _int;
	test_implicit_cast<_int>();

	typedef floating_point_range<double> _double;
	test_implicit_cast<_double>();
}
