#include <gtest/gtest.h>

#include <type_traits>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "test/test.h"

using namespace rant;

typedef integral_range<int>          _int;
typedef floating_point_range<double> _d;

template<typename T>
void integral_test()
{
	ASSERT_THROW(T(-1), std::underflow_error);
	ASSERT_THROW(T(65), std::overflow_error);
	ASSERT_NO_THROW(static_cast<void>(T(64) + T(0)));

	ASSERT_THROW(T(0)--,      std::underflow_error);
	ASSERT_THROW(T(64)+=T(1), std::overflow_error);
	ASSERT_THROW(T(0)-=T(1),  std::underflow_error);
}

TEST(Range, Integral)
{
	integral_test<integral_range<int, 64, 0>>();
#ifndef RANT_DISABLE
	integral_test<debug::integral_range<int, 64, 0>>();
#else
	typedef debug::integral_range<int, 64, 0> t;
	ASSERT_NO_THROW((void)(t(64)+t(1)));
#endif // DISABLE_RANT
}

TEST(Range, UnsignedIntegral)
{
	typedef integral_range<unsigned int, 5, 0> _t;
	ASSERT_NO_THROW(_t(5));
	ASSERT_THROW(_t(-1), std::overflow_error);
	ASSERT_THROW(_t(6), std::overflow_error);

	typedef integral_range<unsigned char, 5, 1> _u;
	ASSERT_NO_THROW(_u(5));
	ASSERT_THROW(_u(0), std::underflow_error);
	ASSERT_THROW(_u(6), std::overflow_error);
}

TEST(Range, FloatingPoint)
{
	typedef floating_point_range<double, std::ratio<10>, std::ratio<0>> t0;

	ASSERT_THROW(t0(-0.1), std::underflow_error);
	ASSERT_THROW(t0(10.1), std::overflow_error);
	ASSERT_NO_THROW(static_cast<void>(t0(0.1)-t0(0.1)));
	ASSERT_NO_THROW(t0(10.0));
}

RANT_TEST_COMMON(Range)
