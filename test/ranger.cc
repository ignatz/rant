#include <gtest/gtest.h>

#include "ranger/ranger.h"

using namespace ranger;

TEST(Ranger, Basics)
{
	range<int> r;

	typedef range<int, 64, 0> t;
	ASSERT_THROW(t(-1), std::underflow_error);
	ASSERT_THROW(t(64), std::overflow_error);

	ASSERT_NO_THROW(t(63)+t(0));
	ASSERT_THROW(t(1)-t(2), std::underflow_error);
	ASSERT_THROW(t(0)--, std::underflow_error);
}

TEST(Ranger, Addition)
{
	typedef range<int> r;
	r a(2);
	r b(1);
	r c = a + b;
	ASSERT_EQ(3, (int)c);
	ASSERT_EQ(r(3), c);

	ASSERT_EQ(--a, ++a);
	ASSERT_EQ(a, a++);
	ASSERT_EQ(a, a--);
}

TEST(Ranger, Division)
{
	typedef range<int> r;
	r a(42);
	r b(2);
	r c = a / b;
	ASSERT_EQ(r(21), c);
}
