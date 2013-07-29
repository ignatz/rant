// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"
#include "test/types.h"

using namespace rant;

TEST(ThrowNoExcept, Int)
{
	typedef throw_on_error<int, ic<int, 4>, ic<int, 0> > sanitizer;
	ASSERT_FALSE(noexcept(sanitizer::overflow(2)));
	ASSERT_FALSE(noexcept(sanitizer::underflow(2)));
}


// generators
template<typename T>
struct integral_throw_extreme :
	public integral_proxy<T,
		std::numeric_limits<T>::max(),
		std::numeric_limits<T>::min(),
		rant::throw_on_error>
{};

template<typename T>
struct integral_throw_off1_0 :
	public integral_proxy<T,
		std::numeric_limits<T>::max()-1,
		std::numeric_limits<T>::min(),
		rant::throw_on_error>
{};

template<typename T>
struct integral_throw_off1_1 :
	public integral_proxy<T,
		std::numeric_limits<T>::max(),
		std::numeric_limits<T>::min()+1,
		rant::throw_on_error>
{};


// span test matrix { base types } x { sanitizers }
typedef typename test_types<
	typename cross_product<
		gens<
			integral_throw_extreme,
			integral_throw_off1_0,
			integral_throw_off1_1>,
		pack<
			bool,
			char,
			signed char,
			unsigned char,
			wchar_t,
			char16_t,
			char32_t,
			short,
			unsigned short,
			int,
			unsigned int,
			long,
			unsigned long,
			long long,
			unsigned long long>
		>::type
	>::type int_types;


template<typename T>
struct ThrowIntBuiltin :
	public ::testing::Test
{
	typedef rant::traits<T> traits;
};

TYPED_TEST_CASE(ThrowIntBuiltin, int_types);

TYPED_TEST(ThrowIntBuiltin, NoExcept)
{
	typedef typename TestFixture::traits traits;

	ASSERT_FALSE(noexcept(TypeParam()));
	ASSERT_FALSE(noexcept(typename traits::callback_type () (traits::min::value)));
}

TYPED_TEST(ThrowIntBuiltin, InRangeInitialization)
{
	typedef typename TestFixture::traits traits;
	typedef TypeParam type;

	ASSERT_EQ(traits::min::value, type(traits::min::value));
	ASSERT_EQ(traits::min::value, type(traits::min::value));

	ASSERT_EQ(traits::max::value, type(traits::max::value));
	ASSERT_EQ(traits::max::value, type(traits::max::value));
}

TYPED_TEST(ThrowIntBuiltin, OverflowInitialization)
{
	typedef typename TestFixture::traits traits;

	{
		auto const max = std::numeric_limits<typename traits::type>::max();
		if (traits::max::value < max) {
			// NOTE: the parantheses around TypeParam(val) are necessary
			ASSERT_THROW((TypeParam(max)), std::overflow_error);
		}
	}
	{
		auto const max = std::numeric_limits<intmax_t>::max();
		if (uintmax_t(traits::max::value) < uintmax_t(max))
			ASSERT_THROW((TypeParam(max)), std::overflow_error) << max;
	}
}

TYPED_TEST(ThrowIntBuiltin, UnderflowInitialization)
{
	typedef typename TestFixture::traits traits;

	{
		auto const min = std::numeric_limits<typename traits::type>::min();
		if (traits::min::value > min) {
			// NOTE: the parantheses around TypeParam(val) are necessary
			ASSERT_THROW((TypeParam(min)), std::underflow_error);
		}
	}

	{
		auto const min = std::numeric_limits<short>::min();
		if (intmax_t(traits::min::value) > min)
			ASSERT_THROW((TypeParam(min)), std::underflow_error) << min;
	}
}


// random tests

template<typename Rant>
class random_test
{
private:
	typedef typename rant::traits<Rant> traits;

	template<typename T>
	struct Functor
	{
		void operator() () const
		{
			REPEAT(10000) {
				T const v = random<T>();
				if (mpint_t(v) > mpint_t(traits::max::value)) {
					ASSERT_THROW((Rant(v)), std::overflow_error);
				} else if (mpint_t(v) < mpint_t(traits::min::value)) {
					ASSERT_THROW((Rant(v)), std::underflow_error);
				} else {
					ASSERT_EQ(v, Rant(v));
				}
			}
		}
	};

public:
	static void run()
	{
		typedef typename traits::type base;

		// call random test for signed ctor arguments
		for_all_types<signed_type<64>, Functor> () ();

		// call random test for unsigned ctor arguments
		for_all_types<unsigned_type<64>, Functor> () ();
	}
};

TEST(ThrowRandom, char)
{
	random_test< rant::integral_range<char, 12, -55> >::run();
}

TEST(ThrowRandom, ushort)
{
	random_test< rant::integral_range<unsigned short, 22022, 324> >::run();
}

TEST(ThrowRandom, int)
{
	random_test< rant::integral_range<int, 43590, -8197> >::run();
}

TEST(ThrowRandom, uintmax)
{
	random_test< rant::integral_range<uintmax_t, 2342432444444, 320945> >::run();
}

TEST(ThrowRandom, intmax)
{
	random_test< rant::integral_range<intmax_t, 612432444444, -231320945> >::run();
}
