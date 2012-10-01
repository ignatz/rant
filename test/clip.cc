#include <gtest/gtest.h>

#include <sstream>
#include <iostream>
#include <typeinfo>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "rant/rant.h"

using namespace rant;

typedef iclip<int>     _int;
typedef fclip<double>  _d;

TEST(Clip, Integral)
{
	typedef iclip<int, 64, 0> t;

	ASSERT_EQ(  t(0), t(-1));
	ASSERT_EQ( t(64), t(65));
	ASSERT_EQ(t(354), t(111));
}


TEST(Clip, FloatingPoint)
{
	typedef fclip<double, std::ratio<10>, std::ratio<0>> t;

	ASSERT_EQ(t(0), t(-0.0));
	ASSERT_EQ(t(0), t(-0.1));

	ASSERT_EQ(t(10.0), t(10.0));
	ASSERT_EQ(t(10.0), t(10.1));
}

TEST(Clip, MinMax)
{
	ASSERT_EQ( 4, (iclip<int, 4, -1>::max()));
	ASSERT_EQ(-1, (iclip<int, 4, -1>::min()));
}

TEST(Clip, Addition)
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

TEST(Clip, Substraction)
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

TEST(Clip, Multiplication)
{
	_int c = _int(42) / _int(2);
	ASSERT_EQ(_int(21), c);
	c*=3;
	ASSERT_EQ(_int(63), c);
}

TEST(Clip, Division)
{
	_int c = _int(21) *_int(2);
	ASSERT_EQ(_int(42), c);
	c/=7;
	ASSERT_EQ(_int(6), c);
}

TEST(Clip, ModuloDivision)
{
	for (int ii = -10; ii<10; ++ii) {
		for (int jj = 1; jj<10; ++jj) {
			ASSERT_EQ(ii % jj, _int(ii) % _int(jj));
		}
	}
}

TEST(Clip, Logic)
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

TEST(Clip, Bit)
{
	ASSERT_EQ(~0xff, ~_int(0xff));

	ASSERT_EQ(42 & 23, _int(42) & _int(23));
	ASSERT_EQ(42 | 23, _int(42) | _int(23));
	ASSERT_EQ(42 ^ 23, _int(42) ^ _int(23));
	ASSERT_EQ(42 << 23, _int(42) << _int(23));
	ASSERT_EQ(42 >> 2, _int(42) >> _int(2));
}

#ifndef RANT_DISABLE_SERIALIZATION
TEST(Clip, Serialization)
{
	std::stringstream s;
	boost::archive::text_oarchive oa(s);

	irange<int> a(42);
	oa << a;

	frange<double> d(3.141);
	oa << d;
}
#endif // RANT_DISABLE_SERIALIZATION
