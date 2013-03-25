#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

#include "rant/rant.h"

#define UNUSED(X) static_cast<void>( X )

const bool rant_disable =
#ifndef RANT_DISABLE
	false;
#else
	true;
#endif // RANT_DISABLE

template<typename NoOpt, typename Opt>
void test_disable()
{
	ASSERT_FALSE((boost::is_arithmetic<NoOpt>::type::value));
	if (rant_disable)
		ASSERT_TRUE((boost::is_arithmetic<Opt>::type::value));
	else
		ASSERT_FALSE((boost::is_arithmetic<Opt>::type::value));
}

template<typename T>
void test_implicit_cast()
{
	typedef typename rant::traits<T>::type type;
	T t(42);
	type b = t;
	UNUSED(b);
}


template<typename T>
void test_explicit_cast()
{
	typedef typename rant::traits<T>::type type;
	T t(42);
	type a = static_cast<type>(t);
	UNUSED(a);
}


template<typename T>
void test_minmax(int max, int min)
{
	if (rant_disable)
		ASSERT_EQ(max, (std::numeric_limits<T>::max()));
		ASSERT_EQ(min, (std::numeric_limits<T>::lowest()));
}


template<typename Int, typename Float>
void test_addition()
{
	Int c = Int(2) + Int(1);
	ASSERT_EQ(3, c);
	c = 2 + Int(1);
	ASSERT_EQ(3, c);
	c = Int(2) + 1;
	ASSERT_EQ(3, c);
	ASSERT_EQ(Int(3), c);

	Int x(2);
	ASSERT_EQ(Int(3), ++x);
	ASSERT_EQ(Int(3), x++);
	ASSERT_EQ(Int(4), x);
	ASSERT_EQ(+(-4), +Int(-4));

	Float y(3.141);
	ASSERT_EQ(4.141, y + Float(1.0));
}


template<typename Int, typename Float>
void test_subtraction()
{
	Int c = Int(3) - Int(1);
	ASSERT_EQ(2, c);
	c = 3 - Int(1);
	ASSERT_EQ(2, c);
	c = Int(3) - 1;
	ASSERT_EQ(2, c);
	ASSERT_EQ(Int(2), c);

	Int x(4);
	ASSERT_EQ(Int(3), --x);
	ASSERT_EQ(Int(3), x--);
	ASSERT_EQ(Int(2), x);
	ASSERT_EQ(-4, -Int(4));

	Float y(3.141);
	ASSERT_EQ(2.141, y - Float(1.0));
}


template<typename Type>
void test_multiplication()
{
	Type c = Type(3) * Type(7);
	ASSERT_EQ(3*7, c);
	c = 3 * Type(7);
	ASSERT_EQ(3*7, c);
	c = Type(3) * 7;
	ASSERT_EQ(3*7, c);
	ASSERT_EQ(Type(21), c);

	c*=3;
	ASSERT_EQ(Type(63), c);
}


template<typename Type>
void test_division()
{
	Type c = Type(84) / Type(2);
	ASSERT_EQ(Type(42), c);
	c = 84 / Type(2);
	ASSERT_EQ(Type(42), c);
	c = 84 / Type(2);
	ASSERT_EQ(Type(42), c);
	ASSERT_EQ(42, c);
	c/=7;
	ASSERT_EQ(Type(6), c);
}


template<typename Int>
void test_modulo()
{
	for (int ii = -10; ii<10; ++ii) {
		for (int jj = 1; jj<10; ++jj) {
			ASSERT_EQ(ii % jj, Int(ii) % Int(jj));
		}
	}
}


template<typename Int>
void test_logic()
{
	ASSERT_FALSE(!Int(5));
	ASSERT_TRUE(!Int(0));

	ASSERT_TRUE(Int(4) && Int(4));
	ASSERT_FALSE(Int(0) && Int(0));
	ASSERT_FALSE(Int(4) && Int(0));
	ASSERT_FALSE(Int(0) && Int(4));

	ASSERT_TRUE(Int(4) || Int(4));
	ASSERT_FALSE(Int(0) || Int(0));
	ASSERT_TRUE(Int(4) || Int(0));
	ASSERT_TRUE(Int(0) || Int(4));
}


template<typename Int>
void test_bitwise()
{
	ASSERT_EQ(~0xff, ~Int(0xff));

	ASSERT_EQ(42 & 23, Int(42) & Int(23));
	ASSERT_EQ(42 | 23, Int(42) | Int(23));
	ASSERT_EQ(42 ^ 23, Int(42) ^ Int(23));
	ASSERT_EQ(42 << 23, Int(42) << Int(23));
	ASSERT_EQ(42 >> 2, Int(42) >> Int(2));

	int i;
	ASSERT_EQ((i=42) <<= 2, Int(42) <<= 2);
	ASSERT_EQ((i=42) >>= 2, Int(42) >>= 2);
	ASSERT_EQ((i=42) ^= 0xff, Int(42) ^= 0xff);
	ASSERT_EQ((i=42) &= 0x07, Int(42) &= 0x07);
	ASSERT_EQ((i=42) |= 0x07, Int(42) |= 0x07);
}


#define RANT_TEST_COMMON(NAME) \
	TEST(NAME, Disable) \
	{ \
		test_disable<_int, _debug::_int>(); \
		test_disable<_d, _debug::_d>(); \
	} \
	TEST(NAME, ExplicitCast) \
	{ \
		test_explicit_cast<_int>(); \
		test_explicit_cast<_d>(); \
	} \
	TEST(NAME, ImplicitCast) \
	{ \
		test_implicit_cast<_int>(); \
		test_implicit_cast<_d>(); \
	} \
	TEST(NAME, Addition) \
	{ \
		test_addition<_int, _d>(); \
	} \
	TEST(NAME, Subtraction) \
	{ \
		test_subtraction<_int, _d>(); \
	} \
	TEST(NAME, Multiplication) \
	{ \
		test_multiplication<_int>(); \
		test_multiplication<_d>(); \
	} \
	TEST(NAME, Division) \
	{ \
		test_division<_int>(); \
		test_division<_d>(); \
	} \
	TEST(NAME, Modulo) \
	{ \
		test_modulo<_int>(); \
	} \
	TEST(NAME, Logic) \
	{ \
		test_logic<_int>(); \
	} \
	TEST(NAME, Bitwise) \
	{ \
		test_bitwise<_int>(); \
	}
