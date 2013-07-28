// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include <gtest/gtest.h>

// the minimal "include" footprint ensures, that all
// necessary headers are available. Otherwise it won't
// compile.
#include <boost/serialization/rant.hpp>
#include <boost/archive/detail/interface_oarchive.hpp>

using namespace rant;

namespace boost {
namespace archive {

struct PseudoArchive :
	public detail::interface_oarchive<PseudoArchive>
{
	template<typename T>
	void save_override(T const&, unsigned const) {}
};

} // archive
} // boost

TEST(SerializationIncludes, Regression)
{
	integral_range<int> i;
	boost::archive::PseudoArchive ar;
	boost::serialization::serialize(ar, i, 0);
}
