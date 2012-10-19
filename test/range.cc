#include <gtest/gtest.h>

#include <type_traits>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "test/test.h"

using namespace rant;

typedef irange<int>::type     _int;
typedef frange<double>::type  _d;

template<typename T>
void integral_test()
{
	ASSERT_THROW(T(-1), std::underflow_error);
	ASSERT_THROW(T(65), std::overflow_error);
	ASSERT_NO_THROW(T(64) + T(0));

	ASSERT_THROW(T(1)-T(2),   std::underflow_error);
	ASSERT_THROW(T(0)--,      std::underflow_error);
	ASSERT_THROW(T(64)+=T(1), std::overflow_error);
	ASSERT_THROW(T(0)-=T(1),  std::underflow_error);
}

TEST(Range, Integral)
{
	integral_test<irange<int, 64, 0>::type>();
#ifndef RANT_DISABLE
	integral_test<opt::irange<int, 64, 0>::type>();
#else
	typedef opt::irange<int, 64, 0>::type t;
	ASSERT_NO_THROW((void)(t(64)+t(1)));
#endif // DISABLE_RANT
}

TEST(Range, UnsignedIntegral)
{
	typedef irange<unsigned int, 5, 0>::type _t;
	ASSERT_NO_THROW(_t(5));
	ASSERT_THROW(_t(-1), std::overflow_error);
	ASSERT_THROW(_t(6), std::overflow_error);

	typedef irange<unsigned char, 5, 1>::type _u;
	ASSERT_NO_THROW(_u(5));
	ASSERT_THROW(_u(0), std::underflow_error);
	ASSERT_THROW(_u(6), std::overflow_error);
}

TEST(Range, FloatingPoint)
{
	typedef range<double, std::ratio<10>, std::ratio<0>>::type t0;
	typedef frange<double, std::ratio<10>>::type t1;

	ASSERT_THROW(t0(-0.1), std::underflow_error);
	ASSERT_THROW(t0(10.1), std::overflow_error);
	ASSERT_NO_THROW(t0(0.1)-t0(0.1));
	ASSERT_NO_THROW(t0(10.0));
}

RANT_TEST_COMMON(Range)

#ifndef RANT_DISABLE_SERIALIZATION
TEST(Range, Serialization)
{
	test_serialization<_int>();
	test_serialization<_d>();
}
#endif // RANT_DISABLE_SERIALIZATION
