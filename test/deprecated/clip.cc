#include <gtest/gtest.h>

#include "test.h"

#define STRIP_PARANTHESIS(...) __VA_ARGS__

#define MAX(TYPE) std::numeric_limits<TYPE>::max()
#define MIN(TYPE) std::numeric_limits<TYPE>::lowest()+1

#define INTEGRAL_RANGE(TYPE, MAX, MIN) \
	integral_range<TYPE, MAX, MIN, \
		clip_on_error<TYPE, \
			std::integral_constant<TYPE, MAX>, \
			std::integral_constant<TYPE, MIN> > >

#define FLOATING_POINT_RANGE(TYPE, MAX, MIN) \
	floating_point_range<TYPE, \
		std::ratio< STRIP_PARANTHESIS MAX >, \
		std::ratio<	STRIP_PARANTHESIS MIN >, \
		clip_on_error<TYPE, \
			std::ratio< STRIP_PARANTHESIS MAX >, \
			std::ratio<	STRIP_PARANTHESIS MIN > > >

using namespace rant;

typedef std::intmax_t intmax_t;

typedef INTEGRAL_RANGE(int, MAX(int), MIN(int)) _int;
typedef FLOATING_POINT_RANGE(double, (MAX(intmax_t),1), (MIN(intmax_t),1)) _d;

namespace _debug {
typedef debug::INTEGRAL_RANGE(int, MAX(int), MIN(int)) _int;
typedef debug::FLOATING_POINT_RANGE(double,
	(MAX(intmax_t),1), (MIN(intmax_t),1)) _d;
}


TEST(Clip, Integral)
{
	typedef INTEGRAL_RANGE(int, 64, 0) t;
	ASSERT_EQ(t(-1), t(-1));
	ASSERT_EQ(t(0), t(-1));
	ASSERT_EQ(t( 65), t( 64));
	ASSERT_EQ(t(111), t(354));

	typedef debug::INTEGRAL_RANGE(int, 64, 0) to;
	ASSERT_EQ(rant_disable ? to( -1) : to(  0), to( -1));
	ASSERT_EQ(rant_disable ? to( 65) : to( 64), to( 65));
	ASSERT_EQ(rant_disable ? to(111) : to(354), to(111));
}

TEST(Clip, UnsignedIntegral)
{
	typedef INTEGRAL_RANGE(unsigned int, 5, 0) t;
	ASSERT_EQ(t(0), t(-1));
	ASSERT_EQ(t(5), t(6));

	typedef INTEGRAL_RANGE(unsigned int, 5, 1) u;
	ASSERT_EQ(u(1), u(0));
	ASSERT_EQ(u(5), u(6));
}

TEST(Clip, FloatingPoint)
{
	typedef FLOATING_POINT_RANGE(double, (10, 1), (0, 1))        t;
	typedef debug::FLOATING_POINT_RANGE(double, (10, 1), (0, 1)) to;

	ASSERT_EQ(t(0), t(-0.0));
	ASSERT_DOUBLE_EQ(rant_disable ? to(-0.1) : to(0), to(-0.1));
	ASSERT_EQ(t(10.0), t(10.0));
	ASSERT_DOUBLE_EQ(rant_disable ? to(10.1) : to(10.0), to(10.1));
}

TEST(Clip, MinMax)
{
	typedef debug::INTEGRAL_RANGE(int, 4, -1) t;
	test_minmax<t>(4, -1);

	typedef debug::FLOATING_POINT_RANGE(double, (4, 1), (-1, 1)) d;
	test_minmax<d>(4, -1);
}

RANT_TEST_COMMON(Clip)
