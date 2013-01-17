#pragma once

// Copyright (c) 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the GPLv2 or newer

#include <ostream>
#include <type_traits>


#define RANT_OP_STREAM(CLASS_NAME, ARG) \
	template<typename T, ARG Max, \
		ARG Min, typename Check> \
	std::ostream& operator<< (std::ostream& o, \
		CLASS_NAME<T, Max, Min, Check> const& a) \
	{ \
		return o << static_cast<T>(a); \
	}


#define RANT_OP_UNARY(OP, RET) \
	inline RET operator OP () const \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(RET) \
	{ \
		return RET(OP RANT_VALUE_NAME); \
	}


#define RANT_OP_ASSIGNMENT(OP) \
	inline type& operator OP##= (type const& x) \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(type) \
	{ \
		RANT_VALUE_NAME = \
			RANT_CHECK(RANT_VALUE_NAME OP x.RANT_VALUE_NAME); \
		return *this; \
	}


#define RANT_OP_BINARY_FF_BUILTIN(CLASS_NAME, ARG, OP, ...) \
	template<typename T, ARG Max, \
		ARG Min, typename Check> \
	inline __VA_ARGS__ operator OP ( \
		CLASS_NAME<T, Max, Min, Check> const& a, \
		CLASS_NAME<T, Max, Min, Check> const& b) \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(__VA_ARGS__) \
	{ \
		return __VA_ARGS__(static_cast<T>(a) OP static_cast<T>(b)); \
	} \
	template<typename U, typename T, ARG Max, \
		ARG Min, typename Check> \
	inline \
	typename std::enable_if< \
		std::is_arithmetic<U>::value || \
		std::is_same<U, T>::value, __VA_ARGS__>::type \
	operator OP ( \
		U const a, \
		CLASS_NAME<T, Max, Min, Check> const& b) \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(__VA_ARGS__) \
	{ \
		return __VA_ARGS__(a OP static_cast<T>(b)); \
	} \
	template<typename U, typename T, ARG Max, \
		ARG Min, typename Check> \
	inline \
	typename std::enable_if< \
		std::is_arithmetic<U>::value || \
		std::is_same<U, T>::value, __VA_ARGS__>::type \
	operator OP ( \
		CLASS_NAME<T, Max, Min, Check> const& a, \
		U const b) \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(__VA_ARGS__) \
	{ \
		return __VA_ARGS__(static_cast<T>(a) OP b); \
	} \
	template<typename T, ARG Max, ARG Min, typename Check, \
		typename T1, T1 Max1, T1 Min1, typename Check1, \
		template<typename, T1, T1, typename> class Range> \
	inline __VA_ARGS__ operator OP ( \
		CLASS_NAME<T, Max, Min, Check> const, \
		Range<T1, Max1, Min1, Check1> const) \
		RANT_IS_NOTHROW_DEFAULT_CONSTR(__VA_ARGS__) \
	{ \
		static_assert(std::is_same< \
			CLASS_NAME<T, Max, Min, Check>, \
			Range<T1, Max1, Min1, Check1>>::value, \
			"ranged types must be of same type"); \
		return __VA_ARGS__(); \
	}
