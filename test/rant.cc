#include <gtest/gtest.h>

#include <sstream>
#include <iostream>
#include <typeinfo>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "rant/rant.h"

using namespace rant;

typedef integral<int>          _int;
typedef floating_point<double> _d;


TEST(Rant, Integral)
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


TEST(Rant, FloatingPoint)
{
	typedef range<double, std::ratio<10>, std::ratio<0>> t0;
	typedef floating_point<double, std::ratio<10>>       t1;

#ifndef RANT_DISABLE
	ASSERT_THROW(t0(-0.1), std::underflow_error);
	ASSERT_THROW(t0(10.1), std::overflow_error);
	ASSERT_NO_THROW(t0(0.1)-t0(0.1));
#endif
}

TEST(Rant, MinMax)
{
	ASSERT_EQ( 4, (integral<int, 4, -1>::max()));
	ASSERT_EQ(-1, (integral<int, 4, -1>::min()));
}

TEST(Rant, Addition)
{
	_int c = _int(2) + _int(1);
	ASSERT_EQ(3, c);
	ASSERT_EQ(_int(3), c);

	_int x(2);
	ASSERT_EQ(_int(3), ++x);
	ASSERT_EQ(_int(3), x++);
	ASSERT_EQ(_int(4), x);
	ASSERT_EQ(+(-4), +_int(-4));

	_d y(3.141);
	ASSERT_DOUBLE_EQ(4.141, y + _d(1.0));
}

TEST(Rant, Substraction)
{
	_int c = _int(3) - _int(1);
	ASSERT_EQ(2, c);
	ASSERT_EQ(_int(2), c);

	_int x(4);
	ASSERT_EQ(_int(3), --x);
	ASSERT_EQ(_int(3), x--);
	ASSERT_EQ(_int(2), x);
	ASSERT_EQ(-4, -_int(4));

	_d y(3.141);
	ASSERT_DOUBLE_EQ(2.141, y - _d(1.0));
}

TEST(Rant, Multiplication)
{
	_int c = _int(42) / _int(2);
	ASSERT_EQ(_int(21), c);
	c*=3;
	ASSERT_EQ(_int(63), c);
}

TEST(Rant, Division)
{
	_int c = _int(21) *_int(2);
	ASSERT_EQ(_int(42), c);
	c/=7;
	ASSERT_EQ(_int(6), c);
}

TEST(Rant, ModuloDivision)
{
	for (int ii = -10; ii<10; ++ii) {
		for (int jj = 1; jj<10; ++jj) {
			ASSERT_EQ(ii % jj, _int(ii) % _int(jj));
		}
	}
}

TEST(Rant, Logic)
{
	ASSERT_FALSE(!_int(5));
	ASSERT_TRUE(!_int(0));

	ASSERT_TRUE(_int(4) && _int(4));
	ASSERT_FALSE(_int(0) && _int(0));
	ASSERT_FALSE(_int(4) && _int(0));
	ASSERT_FALSE(_int(0) && _int(4));

	ASSERT_TRUE(_int(4) || _int(4));
	ASSERT_FALSE(_int(0) || _int(0));
	ASSERT_TRUE(_int(4) || _int(0));
	ASSERT_TRUE(_int(0) || _int(4));
}

TEST(Rant, Bit)
{
	ASSERT_EQ(~0xff, ~_int(0xff));

	ASSERT_EQ(42 & 23, _int(42) & _int(23));
	ASSERT_EQ(42 | 23, _int(42) | _int(23));
	ASSERT_EQ(42 ^ 23, _int(42) ^ _int(23));
	ASSERT_EQ(42 << 23, _int(42) << _int(23));
	ASSERT_EQ(42 >> 2, _int(42) >> _int(2));
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
