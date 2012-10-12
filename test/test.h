#pragma once

#include <gtest/gtest.h>
#include <sstream>

template<typename NoOpt, typename Opt>
void test_disable()
{
	ASSERT_FALSE((std::is_arithmetic<NoOpt>::value));
#ifdef RANT_DISABLE
	ASSERT_TRUE((std::is_arithmetic<Opt>::value));
#else
	ASSERT_FALSE((std::is_arithmetic<Opt>::value));
#endif
}


template<typename T>
void test_minmax()
{
	ASSERT_EQ( 4, (T::max()));
	ASSERT_EQ(-1, (T::min()));
}


template<typename Int, typename Float>
void test_addition()
{
	Int c = Int(2) + Int(1);
	ASSERT_EQ(3, c);
	ASSERT_EQ(Int(3), c);

	Int x(2);
	ASSERT_EQ(Int(3), ++x);
	ASSERT_EQ(Int(3), x++);
	ASSERT_EQ(Int(4), x);
	ASSERT_EQ(+(-4), +Int(-4));

	Float y(3.141);
	ASSERT_DOUBLE_EQ(4.141, y + Float(1.0));
}


template<typename Int, typename Float>
void test_subtraction()
{
	Int c = Int(3) - Int(1);
	ASSERT_EQ(2, c);
	ASSERT_EQ(Int(2), c);

	Int x(4);
	ASSERT_EQ(Int(3), --x);
	ASSERT_EQ(Int(3), x--);
	ASSERT_EQ(Int(2), x);
	ASSERT_EQ(-4, -Int(4));

	Float y(3.141);
	ASSERT_DOUBLE_EQ(2.141, y - Float(1.0));
}


template<typename Int, typename Float>
void test_multiplication()
{
	Int c = Int(3) * Int(7);
	ASSERT_EQ(Int(21), c);
	c*=3;
	ASSERT_EQ(Int(63), c);
}


template<typename Int, typename Float>
void test_division()
{
	Int c = Int(84) / Int(2);
	ASSERT_EQ(Int(42), c);
	c/=7;
	ASSERT_EQ(Int(6), c);
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


template<typename Type>
void test_serialization()
{
	std::stringstream s;
	boost::archive::text_oarchive oa(s);

	Type a(42);
	oa << a;
}


#define RANT_TEST_COMMON(NAME)                          \
	TEST(NAME, Disable)                                 \
	{                                                   \
		test_disable<_int, opt::irange<int>::type>();   \
		test_disable<_d, opt::frange<double>::type>();  \
	}                                                   \
	                                                    \
	TEST(NAME, MinMax)                                  \
	{                                                   \
		test_minmax<irange<int, 4, -1>::type>();        \
	}                                                   \
	                                                    \
	TEST(NAME, Addition)                                \
	{                                                   \
		test_addition<_int, _d>();                      \
	}                                                   \
	                                                    \
	TEST(NAME, Subtraction)                             \
	{                                                   \
		test_subtraction<_int, _d>();                   \
	}                                                   \
	                                                    \
	TEST(NAME, Multiplication)                          \
	{                                                   \
		test_multiplication<_int, _d>();                \
	}                                                   \
	                                                    \
	TEST(NAME, Division)                                \
	{                                                   \
		test_division<_int, _d>();                      \
	}                                                   \
	                                                    \
	TEST(NAME, Modulo)                                  \
	{                                                   \
		test_modulo<_int>();                            \
	}                                                   \
	                                                    \
	TEST(NAME, Logic)                                   \
	{                                                   \
		test_logic<_int>();                             \
	}                                                   \
	                                                    \
	TEST(NAME, Bitwise)                                 \
	{                                                   \
		test_bitwise<_int>();                           \
	}
