#include <gtest/gtest.h>

#define RANT_EXPLICIT_DOWNCAST

#include "rant/rant.h"

using namespace rant;


template<typename T>
void test_explicit_cast()
{
	T t(42);
	typename T::value_type a = static_cast<typename T::value_type>(t);
	static_cast<void>(a);
}

TEST(ImplicitCast, Base)
{
	typedef integral_range<int> _int;
	test_explicit_cast<_int>();

	typedef floating_point_range<double> _double;
	test_explicit_cast<_double>();
}
