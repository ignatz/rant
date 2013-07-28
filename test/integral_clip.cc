// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include "test/test.h"
#include "test/types.h"

using namespace rant;

TEST(ClipNoExcept, Int)
{
	typedef clip_on_error<int, ic<int, 0>, ic<int, 4> > sanitizer;
	ASSERT_TRUE(noexcept(sanitizer::overflow(2)));
	ASSERT_TRUE(noexcept(sanitizer::underflow(2)));
}


// generators
template<typename T>
struct integral_clip_extreme :
	public integral_proxy<T,
		std::numeric_limits<T>::max(),
		std::numeric_limits<T>::min(),
		rant::clip_on_error>
{};

template<typename T>
struct integral_clip_off1_0 :
	public integral_proxy<T,
		std::numeric_limits<T>::max()-1,
		std::numeric_limits<T>::min(),
		rant::clip_on_error>
{};

template<typename T>
struct integral_clip_off1_1 :
	public integral_proxy<T,
		std::numeric_limits<T>::max(),
		std::numeric_limits<T>::min()+1,
		rant::clip_on_error>
{};


// span test matrix { base types } x { sanitizers }
typedef typename test_types<
	typename cross_product<
		gens<
			integral_clip_extreme,
			integral_clip_off1_0,
			integral_clip_off1_1>,
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
struct ClipIntBuiltin :
	public ::testing::Test
{
	typedef rant::traits<T> traits;
};

TYPED_TEST_CASE(ClipIntBuiltin, int_types);

TYPED_TEST(ClipIntBuiltin, NoExcept)
{
	typedef typename TestFixture::traits traits;

	ASSERT_TRUE(noexcept(TypeParam()));
	ASSERT_TRUE(noexcept(typename traits::callback_type () (traits::min::value)));
}

TYPED_TEST(ClipIntBuiltin, InRangeInitialization)
{
	typedef typename TestFixture::traits traits;
	typedef TypeParam type;

	ASSERT_EQ(traits::min::value, type( intmax_t(traits::min::value)));
	ASSERT_EQ(traits::min::value, type(uintmax_t(traits::min::value)));

	ASSERT_EQ(traits::max::value, type( intmax_t(traits::max::value)));
	ASSERT_EQ(traits::max::value, type(uintmax_t(traits::max::value)));
}

TYPED_TEST(ClipIntBuiltin, OverflowInitialization)
{
	typedef typename TestFixture::traits traits;

	{
		auto const max = std::numeric_limits<typename traits::type>::max();
		if (traits::max::value < max)
			ASSERT_EQ(traits::max::value, TypeParam(max));
	}
	{
		auto const max = std::numeric_limits<intmax_t>::max();
		if (uintmax_t(traits::max::value) < uintmax_t(max))
			ASSERT_EQ(traits::max::value, (TypeParam(max)));
	}
}

TYPED_TEST(ClipIntBuiltin, UnderflowInitialization)
{
	typedef typename TestFixture::traits traits;

	{
		auto const min = std::numeric_limits<typename traits::type>::min();
		if (traits::min::value > min)
			ASSERT_EQ(traits::min::value, TypeParam(min));
	}

	{
		auto const min = std::numeric_limits<short>::min();
		if (intmax_t(traits::min::value) > min)
			ASSERT_EQ(traits::min::value, (TypeParam(min)));
	}
}


// random tests

template<typename Rant>
class random_test
{
private:
	typedef Rant type;
	typedef typename rant::traits<type> traits;

	template<typename T>
	struct Functor
	{
		void operator() () const
		{
			typedef typename std::conditional<
					std::is_signed<typename traits::type>::value,
					intmax_t, uintmax_t
				>::type max_t;

			REPEAT(10000) {
				max_t const v = random<T>();
				if (v > max_t(traits::max::value)) {
					ASSERT_EQ(traits::max::value, (type(v)));
				} else if (v < max_t(traits::min::value)) {
					ASSERT_EQ(traits::min::value, (type(v)));
				} else {
					ASSERT_EQ(v, type(v));
				}
			}
		}
	};

public:
	static void run()
	{
		enum : bool { signum = std::is_signed<typename traits::type>::value };

		// call random test for signed ctor arguments
		for_all_types<signed_type<
				// exception is necessary for corner cases
				cond<size_t, signum, 64, 32>::value
			>, Functor> () ();

		// call random test for unsigned ctor arguments
		for_all_types<unsigned_type<
				// exception is necessary for corner cases
				cond<size_t, signum, 32, 64>::value
			>, Functor> () ();
	}
};

template<typename T, T Max, T Min>
using clip = rant::integral_range<T, Max, Min,
	  rant::clip_on_error<T, ic<T, Max>, ic<T, Min> > >;

TEST(ClipRandom, char)
{
	random_test< clip<char, 12, -55> >::run();
}

TEST(ClipRandom, ushort)
{
	random_test< clip<unsigned short, 22022, 324> >::run();
}

TEST(ClipRandom, int)
{
	random_test< clip<int, 43590, -8197> >::run();
}

TEST(ClipRandom, uintmax)
{
	random_test< clip<uintmax_t, 2342432444444, 320945> >::run();
}

TEST(ClipRandom, intmax)
{
	random_test< clip<intmax_t, 612432444444, -231320945> >::run();
}
