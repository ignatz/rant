#include <gtest/gtest.h>

#include "ranger/ranger.h"

using namespace ranger;

TEST(Ranger, Basics)
{
	integral<int> r;

	typedef integral<int, 64, 0> t;
#ifndef RANGER_DISABLE
	ASSERT_THROW(t(-1), std::underflow_error);
	ASSERT_THROW(t(64), std::overflow_error);

	ASSERT_NO_THROW(t(63)+t(0));
	ASSERT_THROW(t(1)-t(2), std::underflow_error);
	ASSERT_THROW(t(0)--, std::underflow_error);
#endif
}

TEST(Ranger, Addition)
{
	typedef integral<int> r;
	r a(2);
	r b(1);
	r c = a + b;
	ASSERT_EQ(3, (int)c);
	ASSERT_EQ(r(3), c);

	ASSERT_EQ(--a, ++a);
	ASSERT_EQ(r(2), a++);
	ASSERT_EQ(r(3), a);
	ASSERT_EQ(r(3), a--);
	ASSERT_EQ(r(2), a);
}

TEST(Ranger, Multiplication)
{
	typedef integral<int> r;
	r c = r(42) / r(2);
	ASSERT_EQ(r(21), c);
	c*=3;
	ASSERT_EQ(r(63), c);
}

TEST(Ranger, Division)
{
	typedef integral<int> r;
	r c = r(21) * r(2);
	ASSERT_EQ(r(42), c);
	c/=7;
	ASSERT_EQ(r(6), c);
}

TEST(Ranger, FloatingPoint)
{
	typedef range<double, std::ratio<10>, std::ratio<0>> t;
	typedef floating_point<double, std::ratio<10>> t1;
}
