#include <gtest/gtest.h>

#include <sstream>
#include <typeinfo>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "test/test.h"

constexpr bool disable =
#ifndef RANT_DISABLE
	false;
#else
	true;
#endif // RANT_DISABLE

using namespace rant;

typedef iclip<int>::type     _int;
typedef fclip<double>::type  _d;

TEST(Clip, Integral)
{
	typedef iclip<int, 64, 0>::type t;
	ASSERT_EQ(t(-1), t(-1));
	ASSERT_EQ(t(0), t(-1));
	ASSERT_EQ(t( 65), t( 64));
	ASSERT_EQ(t(111), t(354));

	typedef opt::iclip<int, 64, 0>::type to;
	ASSERT_EQ(disable ? to( -1) : to(  0), to( -1));
	ASSERT_EQ(disable ? to( 65) : to( 64), to( 65));
	ASSERT_EQ(disable ? to(111) : to(354), to(111));
}


TEST(Clip, FloatingPoint)
{
	typedef fclip<double, std::ratio<10>, std::ratio<0>>::type      t;
	typedef opt::fclip<double, std::ratio<10>, std::ratio<0>>::type to;

	ASSERT_EQ(t(0), t(-0.0));
	ASSERT_EQ(disable ? to(-0.1) : to(0), to(-0.1));
	ASSERT_EQ(t(10.0), t(10.0));
	ASSERT_EQ(disable ? to(10.1) : to(10.0), to(10.1));
}

RANT_TEST_COMMON(Clip)

#ifndef RANT_DISABLE_SERIALIZATION
TEST(Clip, Serialization)
{
	test_serialization<_int>();
	test_serialization<_d>();
}
#endif // RANT_DISABLE_SERIALIZATION
