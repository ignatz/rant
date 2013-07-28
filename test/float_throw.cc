// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"
#include "test/types.h"

using namespace rant;

TEST(ThrowNoExcept, Float)
{
	typedef throw_on_error<float,
			std::ratio<4>,
			std::ratio<0>
		> sanitizer;
	ASSERT_FALSE(noexcept(sanitizer::overflow(2)));
	ASSERT_FALSE(noexcept(sanitizer::underflow(2)));
}


// generators
template<typename T>
struct float_throw_extreme :
	public float_proxy<T,
		std::ratio< std::numeric_limits<intmax_t>::max()>,
		std::ratio<-std::numeric_limits<intmax_t>::max()>,
		rant::throw_on_error>
{};

template<typename T>
struct float_throw_off1 :
	public float_proxy<T,
		std::ratio< std::numeric_limits<intmax_t>::max()-1>,
		std::ratio<-std::numeric_limits<intmax_t>::max()+1>,
		rant::throw_on_error>
{};


// span test matrix { base types } x { sanitizers }
typedef typename test_types<
	typename cross_product<
		gens<float_throw_extreme, float_throw_off1>,
		pack<float, double, long double>
		>::type
	>::type float_types;


template<typename T>
struct ThrowFloatBuiltin :
	public ::testing::Test
{
	typedef rant::traits<T> traits;
	typedef typename traits::type base;

	static base const min;
	static base const max;
};

template<typename T>
typename ThrowFloatBuiltin<T>::base const ThrowFloatBuiltin<T>::min
	= rant::value_helper<base, typename traits::min>::get();
template<typename T>
typename ThrowFloatBuiltin<T>::base const ThrowFloatBuiltin<T>::max
	= rant::value_helper<base, typename traits::max>::get();

TYPED_TEST_CASE(ThrowFloatBuiltin, float_types);

TYPED_TEST(ThrowFloatBuiltin, NoExcept)
{
	typedef typename TestFixture::traits traits;

	ASSERT_FALSE(noexcept(TypeParam()));
	ASSERT_FALSE(noexcept(typename traits::callback_type () (0.)));
}

TYPED_TEST(ThrowFloatBuiltin, InRangeInitialization)
{
	ASSERT_EQ(TestFixture::min, TypeParam(TestFixture::min));
	ASSERT_EQ(TestFixture::max, TypeParam(TestFixture::max));
}

TYPED_TEST(ThrowFloatBuiltin, OverflowInitialization)
{
	typedef typename TestFixture::traits traits;

	auto const max = std::numeric_limits<intmax_t>::max();
	if (TestFixture::max < max) {
		// NOTE: the parantheses around TypeParam(val) are necessary
		ASSERT_THROW((TypeParam(max)), std::overflow_error);
	}
}

TYPED_TEST(ThrowFloatBuiltin, UnderflowInitialization)
{
	typedef typename TestFixture::traits traits;

	auto const min = std::numeric_limits<intmax_t>::min();
	// this if clause is actually, always true, because the minimal lower
	// bound which can be assigned to floating_point_ranges is
	// `-numeric_limits<intmax_t>::max()` (=numeric_limits<intmax_t>::min()+1).
	if (TestFixture::min > min) {
		// NOTE: the parantheses around TypeParam(val) are necessary
		ASSERT_THROW((TypeParam(min)), std::underflow_error);
	}
}


// random tests

template<typename Rant>
class random_test
{
private:
	typedef Rant type;
	typedef typename rant::traits<type> traits;

	template<typename Val>
	static typename traits::type get()
	{
		return value_helper<typename traits::type, Val>::get();
	}

	template<typename T>
	static void verify ()
	{
		REPEAT(10000) {
			auto const v = random<T>();
			if (v > get<typename traits::max>()) {
				ASSERT_THROW((type(v)), std::overflow_error);
			} else if (v < get<typename traits::min>()) {
				ASSERT_THROW((type(v)), std::underflow_error);
			} else {
				ASSERT_FLOAT_EQ(v, type(v));
			}
		}
	}

public:
	static void run()
	{
		verify<float>();
		verify<double>();
		verify<long double>();
	}
};

TEST(ThrowRandom, float)
{
	random_test<rant::floating_point_range<
			float, std::ratio<329880204, 2>, std::ratio<-4955, 5> >
		>::run();
}

TEST(ThrowRandom, double)
{
	random_test<rant::floating_point_range<
			double, std::ratio<50802198000, 3>, std::ratio<-3244455, 2> >
		>::run();
}

TEST(ThrowRandom, longdouble)
{
	random_test<rant::floating_point_range<
			long double,
			std::ratio<198558858580001, 7>,
			std::ratio<-339087484888, 2> >
		>::run();
}
