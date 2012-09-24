#pragma once

#include <limits>
#include <type_traits>

#ifdef NDEBUG
#pragma message "NDEBUG is set; no range checking"
#define RANGER_CHECK(VAL)
#define RANGER_CONSTEXPR constexpr
#else
#include <stdexcept>
#define RANGER_CHECK(VAL) \
	if (!(VAL >= Min)) \
		throw std::underflow_error(""); \
	else if (!(VAL <  Max)) \
		throw std::overflow_error("");
#define RANGER_CONSTEXPR
#endif // NDEBUG

namespace ranger {

template<typename T = int,
	T Max = std::numeric_limits<T>::max(),
	T Min = std::numeric_limits<T>::min()>
struct range
{
	static_assert(std::is_integral<T>::value, "T is no integral type");
	static_assert(Max >= Min, "Max must be >= Min");

	typedef range<T, Max, Min> self;

	RANGER_CONSTEXPR
	range(T val = T()) :
		_val(val)
	{
		RANGER_CHECK(_val);
	}

	operator T () const noexcept
	{
		return _val;
	}

	self operator+ (self x) const
	{
		return self(_val + x);
	}

	self operator- (self x) const
	{
		return self(_val - x);
	}

	self operator* (self x) const
	{
		return self(_val * x);
	}

	self operator/ (self x) const
	{
		return self(_val / x);
	}

	void operator+= (self x)
	{
		RANGER_CHECK(_val+x);
		_val += x;
	}

	void operator-= (self x)
	{
		RANGER_CHECK(_val-x);
		_val -= x;
	}

	// prefix ++/-- operators
	self& operator++ ()
	{
		_val += 1;
		return *this;
	}

	self& operator-- ()
	{
		_val -= 1;
		return *this;
	}

	// postfix ++/-- operators
	self operator++ (int)
	{
		self t(*this);
		self(_val + 1);
		return t;
	}

	self operator-- (int)
	{
		self t(*this);
		self(_val - 1);
		return t;
	}

private:
	T _val;
};


template<typename T, T Max, T Min>
bool operator== (range<T, Max, Min> a, range<T, Max, Min> b) noexcept
{
	return static_cast<T>(a) == static_cast<T>(b);
}

template<typename T, T Max, T Min>
bool operator< (range<T, Max, Min> a, range<T, Max, Min> b) noexcept
{
	return static_cast<T>(a) < static_cast<T>(b);
}

template<typename T, T Max, T Min>
bool operator!= (range<T, Max, Min> a, range<T, Max, Min> b) noexcept
{
	return !(a==b);
}

template<typename T, T Max, T Min>
bool operator> (range<T, Max, Min> a, range<T, Max, Min> b) noexcept
{
	return b < a;
}

template<typename T, T Max, T Min>
bool operator<= (range<T, Max, Min> a, range<T, Max, Min> b) noexcept
{
	return !(b<a);
}

template<typename T, T Max, T Min>
bool operator>= (range<T, Max, Min> a, range<T, Max, Min> b) noexcept
{
	return !(a<b);
}

} // namespace ranger
