#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include "rant/rant.h"

namespace boost {
namespace serialization {

template<typename Archiver, typename T,
	typename Max, typename Min, T(*Check)(T), typename Enable>
void load(Archiver& ar,
	rant::range<T, Max, Min, Check, Enable>& val,
	unsigned int const)
{
	ar >> make_nvp("value", reinterpret_cast<T&>(val));
}

template<typename Archiver, typename T,
	typename Max, typename Min, T(*Check)(T), typename Enable>
void save(Archiver& ar,
	rant::range<T, Max, Min, Check, Enable> const& val,
	unsigned int const)
{
	ar << make_nvp("value", reinterpret_cast<T const&>(val));
}

template<typename Archiver, typename T,
	typename Max, typename Min, T(*Check)(T), typename Enable>
void serialize(Archiver& ar,
	rant::range<T, Max, Min, Check, Enable>& val,
	unsigned int const version)
{
	split_free(ar, val, version);
}

} // namespace serialization
} // namespace boost
