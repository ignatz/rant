#pragma once

// Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <ostream>
#include <type_traits>


#define RANT_OP_STREAM(CLASS_NAME, ARG) \
	template<typename T, ARG Max, \
		ARG Min, typename Check> \
	std::ostream& operator<< (std::ostream& o, \
		CLASS_NAME<T, Max, Min, Check> const a) \
	{ \
		return o << static_cast<T>(a); \
	}


#define RANT_OP_UNARY(RET, OP) \
	inline RET operator OP () const \
		noexcept(noexcept(type())) \
	{ \
		return RET(OP RANT_VALUE_NAME); \
	}


#define RANT_OP_ASSIGNMENT(OP) \
	inline type& operator OP##= (type const x) \
		noexcept(noexcept(type())) \
	{ \
		RANT_VALUE_NAME = \
			RANT_CHECK(RANT_VALUE_NAME OP x.RANT_VALUE_NAME); \
		return *this; \
	}


#define RANT_OP_BINARY_FF_BUILTIN(CLASS_NAME, RET, ARG, OP) \
	template<typename T, ARG Max, \
		ARG Min, typename Check> \
	inline RET operator OP ( \
		CLASS_NAME<T, Max, Min, Check> const a, \
		CLASS_NAME<T, Max, Min, Check> const b) \
		noexcept(std::is_nothrow_constructible<RET>::value) \
	{ \
		return RET(static_cast<T>(a) OP static_cast<T>(b)); \
	} \
	template<typename U, typename T, ARG Max, \
		ARG Min, typename Check> \
	inline \
	typename std::enable_if< \
		std::is_arithmetic<U>::value || \
		std::is_same<U, T>::value, RET>::type \
	operator OP ( \
		U const a, \
		CLASS_NAME<T, Max, Min, Check> const b) \
		noexcept(std::is_nothrow_constructible<RET>::value) \
	{ \
		return RET(a OP static_cast<T>(b)); \
	} \
	template<typename U, typename T, ARG Max, \
		ARG Min, typename Check> \
	inline \
	typename std::enable_if< \
		std::is_arithmetic<U>::value || \
		std::is_same<U, T>::value, RET>::type \
	operator OP ( \
		CLASS_NAME<T, Max, Min, Check> const a, \
		U const b) \
		noexcept(std::is_nothrow_constructible<RET>::value) \
	{ \
		return RET(static_cast<T>(a) OP b); \
	}
