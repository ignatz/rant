#include <gtest/gtest.h>

#include <sstream>
#include <iostream>
#include <typeinfo>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "rant/rant.h"

using namespace rant;

typedef irange<int>::type     _int;
typedef frange<double>::type  _d;


TEST(Range, Integral)
{
	typedef irange<int, 64, 0>::type t;
	ASSERT_THROW(t(-1), std::underflow_error);
	ASSERT_THROW(t(65), std::overflow_error);
	ASSERT_NO_THROW(t(64)+t(0));

	ASSERT_THROW(t(1)-t(2), std::underflow_error);
	ASSERT_THROW(t(0)--, std::underflow_error);
	ASSERT_THROW(t(64)+=t(1), std::overflow_error);
	ASSERT_THROW(t(0) -=t(1), std::underflow_error);

#ifndef RANT_DISABLE
	typedef opt::irange<int, 64, 0>::type to;
	ASSERT_THROW(to(-1), std::underflow_error);
	ASSERT_THROW(to(65), std::overflow_error);
	ASSERT_NO_THROW(to(64)+to(0));

	ASSERT_THROW(to(1)-to(2), std::underflow_error);
	ASSERT_THROW(to(0)--, std::underflow_error);
	ASSERT_THROW(to(64)+=to(1), std::overflow_error);
	ASSERT_THROW(to(0) -=to(1), std::underflow_error);
#endif // DISABLE_RANT
}


TEST(Range, FloatingPoint)
{
	typedef range<double, std::ratio<10>, std::ratio<0>>::type t0;
	typedef frange<double, std::ratio<10>>::type t1;

//#ifndef RANT_DISABLE
	ASSERT_THROW(t0(-0.1), std::underflow_error);
	ASSERT_THROW(t0(10.1), std::overflow_error);
	ASSERT_NO_THROW(t0(0.1)-t0(0.1));
	ASSERT_NO_THROW(t0(10.0));
//#endif
}

TEST(Range, MinMax)
{
//#ifndef RANT_DISABLE
	ASSERT_EQ( 4, (irange<int, 4, -1>::type::max()));
	ASSERT_EQ(-1, (irange<int, 4, -1>::type::min()));
//#endif
}

TEST(Range, Addition)
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

TEST(Range, Substraction)
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

TEST(Range, Multiplication)
{
	_int c = _int(42) / _int(2);
	ASSERT_EQ(_int(21), c);
	c*=3;
	ASSERT_EQ(_int(63), c);
}

TEST(Range, Division)
{
	_int c = _int(21) *_int(2);
	ASSERT_EQ(_int(42), c);
	c/=7;
	ASSERT_EQ(_int(6), c);
}

TEST(Range, ModuloDivision)
{
	for (int ii = -10; ii<10; ++ii) {
		for (int jj = 1; jj<10; ++jj) {
			ASSERT_EQ(ii % jj, _int(ii) % _int(jj));
		}
	}
}

TEST(Range, Logic)
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

TEST(Range, Bit)
{
	ASSERT_EQ(~0xff, ~_int(0xff));

	ASSERT_EQ(42 & 23, _int(42) & _int(23));
	ASSERT_EQ(42 | 23, _int(42) | _int(23));
	ASSERT_EQ(42 ^ 23, _int(42) ^ _int(23));
	ASSERT_EQ(42 << 23, _int(42) << _int(23));
	ASSERT_EQ(42 >> 2, _int(42) >> _int(2));

	int i;
	ASSERT_EQ((i=42) <<= 2, _int(42) <<= 2);
	ASSERT_EQ((i=42) >>= 2, _int(42) >>= 2);
	ASSERT_EQ((i=42) ^= 0xff, _int(42) ^= 0xff);
	ASSERT_EQ((i=42) &= 0x07, _int(42) &= 0x07);
	ASSERT_EQ((i=42) |= 0x07, _int(42) |= 0x07);
}

#ifndef RANT_DISABLE_SERIALIZATION
TEST(Range, Serialization)
{
	std::stringstream s;
	boost::archive::text_oarchive oa(s);

	irange<int>::type a(42);
	oa << a;

	frange<double>::type d(3.141);
	oa << d;
}
#endif // RANT_DISABLE_SERIALIZATION
