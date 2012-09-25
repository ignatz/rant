#include <gtest/gtest.h>

#include <sstream>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "rant/rant.h"

using namespace rant;

TEST(Rant, Basics)
{
	typedef integral<int, 64, 0> t;
#ifndef RANT_DISABLE
	ASSERT_THROW(t(-1), std::underflow_error);
	ASSERT_THROW(t(64), std::overflow_error);
	ASSERT_NO_THROW(t(63)+t(0));

	ASSERT_THROW(t(1)-t(2), std::underflow_error);
	ASSERT_THROW(t(0)--, std::underflow_error);
#endif
}

TEST(Rant, Addition)
{
	typedef integral<int> r;

	r c = r(2) + r(1);
	ASSERT_EQ(3, (int)c);
	ASSERT_EQ(r(3), c);

	r x(2);
	ASSERT_EQ(r(3), ++x);
	ASSERT_EQ(r(3), x++);
	ASSERT_EQ(r(4), x);

	ASSERT_EQ(r(3), --x);
	ASSERT_EQ(r(3), x--);
	ASSERT_EQ(r(2), x);
}

TEST(Rant, Multiplication)
{
	typedef integral<int> r;
	r c = r(42) / r(2);
	ASSERT_EQ(r(21), c);
	c*=3;
	ASSERT_EQ(r(63), c);
}

TEST(Rant, Division)
{
	typedef integral<int> r;
	r c = r(21) * r(2);
	ASSERT_EQ(r(42), c);
	c/=7;
	ASSERT_EQ(r(6), c);
}

TEST(Rant, FloatingPoint)
{
	typedef range<double, std::ratio<10>, std::ratio<0>> t;
	typedef floating_point<double, std::ratio<10>> t1;
}

#ifndef __WITHOUT_BOOST__
TEST(Rant, Serialization)
{
	std::stringstream s;
	boost::archive::text_oarchive oa(s);

	integral<int> a(42);
	oa << a;

	floating_point<double> d(3.141);
	oa << d;
}
#endif // __WITHOUT_BOOST__
