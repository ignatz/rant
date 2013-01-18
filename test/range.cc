#include <gtest/gtest.h>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "test/test.h"

typedef ::rant::integral_range<int>          _int;
typedef ::rant::floating_point_range<double> _d;

namespace _debug {
	typedef ::rant::debug::integral_range<int>          _int;
	typedef ::rant::debug::floating_point_range<double> _d;
}

using namespace rant;

template<typename T>
void integral_test()
{
	ASSERT_THROW(T(-1), std::underflow_error);
	ASSERT_THROW(T(65), std::overflow_error);
	ASSERT_NO_THROW(static_cast<void>(T(64) + T(0)));

	ASSERT_THROW(T(0)--,      std::underflow_error);
	ASSERT_THROW(T(64)+=T(1), std::overflow_error);
	ASSERT_THROW(T(0)-=T(1),  std::underflow_error);

	//testing::StaticAssertTypeEq<T, decltype(T(4) + 4)>();
	//testing::StaticAssertTypeEq<T, decltype(4 + T(4))>();
}

TEST(Range, Integral)
{
	integral_test<integral_range<int, 64, 0> >();
#ifndef RANT_DISABLE
	integral_test<debug::integral_range<int, 64, 0> >();
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
	typedef floating_point_range<double, boost::ratio<10>, boost::ratio<0> > t0;

	ASSERT_THROW(t0(-0.1), std::underflow_error);
	ASSERT_THROW(t0(10.1), std::overflow_error);
	ASSERT_NO_THROW(static_cast<void>(t0(0.1)-t0(0.1)));
	ASSERT_NO_THROW(t0(10.0));
}

TEST(Range, MinMax)
{
	test_minmax<debug::integral_range<int, 4, -1> >(4, -1);

	typedef debug::floating_point_range<double,
			boost::ratio<4, 1>,
			boost::ratio<-1, 1> > __d;
	test_minmax<__d>(4, -1);
}

TEST(Range, MixedTypes)
{
	// this MUST not compile, therefore commented out
	//rant::integral_range<int, 7, -1>(5) + rant::integral_range<char, 7, -1>(2);
}

RANT_TEST_COMMON(Range)
