#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include "rant/util.h"
#include "rant/check.h"
#include "rant/op.h"

#define RANT_OP_BINARY_FF(RET, OP)                              \
	template<typename T, typename Max,                          \
		typename Min, typename Check>                           \
	inline RET operator OP (                                    \
		range<T, Max, Min, Check> const& a,                     \
		range<T, Max, Min, Check> const& b)                     \
			noexcept(std::is_nothrow_constructible<RET>::value) \
	{                                                           \
		return RET(static_cast<T const&>(a) OP                  \
				   static_cast<T const&>(b));                   \
	}                                                           \
	template<typename U, typename T, typename Max,              \
		typename Min, typename Check>                           \
	inline                                                      \
	typename std::enable_if<                                    \
		std::is_same<U, T>::value, RET>::type                   \
	operator OP (                                               \
		U const& a,                                             \
		range<T, Max, Min, Check> const& b)                     \
			noexcept(std::is_nothrow_constructible<RET>::value) \
	{                                                           \
		return RET(a OP static_cast<T const&>(b));              \
	}                                                           \
	template<typename U, typename T, typename Max,              \
		typename Min, typename Check>                           \
	inline                                                      \
	typename std::enable_if<                                    \
		std::is_same<U, T>::value, RET>::type                   \
	operator OP (                                               \
		range<T, Max, Min, Check> const& a,                     \
		U const& b)                                             \
			noexcept(std::is_nothrow_constructible<RET>::value) \
	{                                                           \
		return RET(static_cast<T const&>(a) OP b);              \
	}

namespace rant {

template<typename T, typename Max, typename Min,
	typename Check = throw_on_error<T, Max, Min>>
struct range : public T
{
	inline
	RANT_EXPLICIT operator T& ()
	{
		return static_cast<T&>(*this);
	}

	inline
	RANT_EXPLICIT operator T const& () const
	{
		return static_cast<T const&>(*this);
	}

	template<typename ... Args>
	range(Args&& ... args) :
		T(RANT_CHECK(std::forward<Args>(args)...)) {}
} RANT_PACKED;

RANT_OP_BINARY_FF(bool, <)

} // rant
