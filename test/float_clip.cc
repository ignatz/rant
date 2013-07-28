// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"
#include "test/types.h"

using namespace rant;

TEST(ClipNoExcept, Float)
{
	typedef clip_on_error<float,
			std::ratio<4>,
			std::ratio<0>
		> sanitizer;
	ASSERT_TRUE(noexcept(sanitizer::overflow(2)));
	ASSERT_TRUE(noexcept(sanitizer::underflow(2)));
}


// generators
template<typename T>
struct float_clip_extreme :
	public float_proxy<T,
		std::ratio< std::numeric_limits<intmax_t>::max()>,
		std::ratio<-std::numeric_limits<intmax_t>::max()>,
		rant::clip_on_error>
{};

template<typename T>
struct float_clip_off1 :
	public float_proxy<T,
		std::ratio< std::numeric_limits<intmax_t>::max()-1>,
		std::ratio<-std::numeric_limits<intmax_t>::max()+1>,
		rant::clip_on_error>
{};


// span test matrix { base types } x { sanitizers }
typedef typename test_types<
	typename cross_product<
		gens<float_clip_extreme, float_clip_off1>,
		pack<float, double, long double>
		>::type
	>::type float_types;


template<typename T>
struct ClipFloatBuiltin :
	public ::testing::Test
{
	typedef rant::traits<T> traits;
	typedef typename traits::type base;

	static base const min;
	static base const max;
};

template<typename T>
typename ClipFloatBuiltin<T>::base const ClipFloatBuiltin<T>::min
	= rant::value_helper<base, typename traits::min>::get();
template<typename T>
typename ClipFloatBuiltin<T>::base const ClipFloatBuiltin<T>::max
	= rant::value_helper<base, typename traits::max>::get();

TYPED_TEST_CASE(ClipFloatBuiltin, float_types);

TYPED_TEST(ClipFloatBuiltin, NoExcept)
{
	typedef typename TestFixture::traits traits;

	ASSERT_TRUE(noexcept(TypeParam()));
	ASSERT_TRUE(noexcept(typename traits::callback_type () (0.)));
}

TYPED_TEST(ClipFloatBuiltin, InRangeInitialization)
{
	ASSERT_EQ(TestFixture::min, TypeParam(TestFixture::min));
	ASSERT_EQ(TestFixture::max, TypeParam(TestFixture::max));
}

TYPED_TEST(ClipFloatBuiltin, OverflowInitialization)
{
	typedef typename TestFixture::traits traits;
	typedef TypeParam type;

	auto const max = std::numeric_limits<intmax_t>::max();
	if (TestFixture::max < max)
		ASSERT_EQ(TestFixture::max, TypeParam(max));
}

TYPED_TEST(ClipFloatBuiltin, UnderflowInitialization)
{
	typedef typename TestFixture::traits traits;

	auto const min = std::numeric_limits<intmax_t>::min();
	// this if clause is actually, always true, because the minimal lower
	// bound which can be assigned to floating_point_ranges is
	// `-numeric_limits<intmax_t>::max()` (=numeric_limits<intmax_t>::min()+1).
	if (TestFixture::min > min)
		ASSERT_EQ(TestFixture::min, TypeParam(min));
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
				ASSERT_EQ(get<typename traits::max>(), type(v));
			} else if (v < get<typename traits::min>()) {
				ASSERT_EQ(get<typename traits::min>(), type(v));
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

TEST(ClipRandom, float)
{
	typedef std::ratio<329880204, 2> max;
	typedef std::ratio<-4955, 5> min;
	random_test<rant::floating_point_range<float, max, min,
			rant::clip_on_error<float, max, min> >
		>::run();
}

TEST(ClipRandom, double)
{
	typedef std::ratio<50802198000, 3> max;
	typedef std::ratio<-3244455, 2> min;
	random_test<rant::floating_point_range<double, max, min,
			rant::clip_on_error<double, max, min> >
		>::run();
}

TEST(ClipRandom, longdouble)
{
	typedef std::ratio<198558858580001, 7> max;
	typedef std::ratio<-339087484888, 2> min;
	random_test<rant::floating_point_range<long double, max, min,
			rant::clip_on_error<long double, max, min> >
		>::run();
}
