#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/base_object.hpp>

#include "rant/rant.h"

#define RANT_ARITHMETIC_SERIALIZER(CLASS_NAME, TYPE) \
template<typename Archiver, typename T, TYPE Max, \
	TYPE Min, typename Check> \
void load(Archiver& ar, \
	CLASS_NAME<T, Max, Min, Check>& s, \
	unsigned int const) \
{ \
	T t; \
	ar >> make_nvp("value", t); \
	s = CLASS_NAME<T, Max, Min, Check>(t); \
} \
\
template<typename Archiver, typename T, TYPE Max, \
	TYPE Min, typename Check> \
void save(Archiver& ar, \
	CLASS_NAME<T, Max, Min, Check> const& s, \
	unsigned int const) \
{ \
	T t = static_cast<T>(s); \
	ar << make_nvp("value", t); \
} \
\
template<typename Archiver, typename T, TYPE Max, \
	TYPE Min, typename Check> \
void serialize(Archiver& ar, \
	CLASS_NAME<T, Max, Min, Check>& s, \
	unsigned int const version) \
{ \
	split_free(ar, s, version); \
}

namespace boost {
namespace serialization {

RANT_ARITHMETIC_SERIALIZER(::rant::integral_range, T)
RANT_ARITHMETIC_SERIALIZER(::rant::floating_point_range, typename)

} // serialization
} // boost
