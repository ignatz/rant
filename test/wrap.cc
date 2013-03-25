#include <gtest/gtest.h>

#include "test/test.h"

#define STRIP_PARANTHESIS(...) __VA_ARGS__

#define MAX(TYPE) boost::integer_traits<TYPE>::const_max-1
#define MIN(TYPE) boost::integer_traits<TYPE>::const_min+1

#define INTEGRAL_RANGE(TYPE, MAX, MIN) \
	integral_range<TYPE, MAX, MIN, \
		wrap_on_error<TYPE, \
			integral_constant<TYPE, MAX>, \
			integral_constant<TYPE, MIN> > >

#define FLOATING_POINT_RANGE(TYPE, MAX, MIN) \
	floating_point_range<TYPE, \
		boost::ratio< STRIP_PARANTHESIS MAX >, \
		boost::ratio< STRIP_PARANTHESIS MIN >, \
		wrap_on_error<TYPE, \
			boost::ratio< STRIP_PARANTHESIS MAX >, \
			boost::ratio< STRIP_PARANTHESIS MIN > > >

using namespace rant;

typedef boost::intmax_t intmax_t;

typedef INTEGRAL_RANGE(int, MAX(int), MIN(int)) _int;
typedef FLOATING_POINT_RANGE(double, (8,1), (5,1)) _d;

namespace _debug {
typedef debug::INTEGRAL_RANGE(int, MAX(int), MIN(int)) _int;
typedef debug::FLOATING_POINT_RANGE(double,
	(MAX(intmax_t),1), (MIN(intmax_t),1)) _d;
}


TEST(Wrap, Integral)
{
	typedef INTEGRAL_RANGE(int, 64, 0) t;
	ASSERT_EQ(t(-1), t(-1));
	ASSERT_EQ(t(64), t(-1));
	ASSERT_EQ(64, t(-1));
	ASSERT_EQ(t( 0), t( 65));
	ASSERT_EQ(0, t( 65));
	ASSERT_EQ(64, t(64+65));
	ASSERT_EQ(t(64), t(64 + 3*65));

	typedef debug::INTEGRAL_RANGE(int, 64, 0) to;
	ASSERT_EQ(rant_disable ? to( -1) : to( 64), to( -1));
	ASSERT_EQ(rant_disable ? to( 65) : to(  0), to( 65));
	ASSERT_EQ(rant_disable ? to( 88) : to( 23), to(64+24));
}

TEST(Wrap, UnsignedIntegral)
{
	typedef INTEGRAL_RANGE(unsigned int, 5, 0) t;
	ASSERT_NE(-1, t(-1));
	ASSERT_EQ(t(0), t(6));

	typedef INTEGRAL_RANGE(unsigned int, 5, 1) u;
	ASSERT_EQ(u(1), u(6));
}

TEST(Wrap, FloatingPoint)
{
	typedef FLOATING_POINT_RANGE(double, (10, 1), (0, 1))        t;
	typedef debug::FLOATING_POINT_RANGE(double, (10, 1), (0, 1)) to;

	ASSERT_EQ(t(0), t(-0.0));
	ASSERT_EQ(t(10.0), t(10.0));
	ASSERT_DOUBLE_EQ(rant_disable ? to(-0.1) : to(9.9), to(-0.1));
	ASSERT_DOUBLE_EQ(rant_disable ? to(10.25) : to(0.25), to(10.25));
}

TEST(Wrap, MinMax)
{
	typedef debug::INTEGRAL_RANGE(int, 4, -1) t;
	test_minmax<t>(4, -1);

	typedef debug::FLOATING_POINT_RANGE(double, (4, 1), (-1, 1)) d;
	test_minmax<d>(4, -1);
}
