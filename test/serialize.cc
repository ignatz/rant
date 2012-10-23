#include <gtest/gtest.h>

#include <sstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "rant/rant.h"
#include "boost/serialization/rant.hpp"

using namespace rant;

typedef irange<int>::type        _int;
typedef irange<char>::type       _char;
typedef irange<long long>::type  _ll;
typedef frange<double>::type     _d;

template<typename OArchive, typename IArchive, typename T>
void test_serialization(T const& t)
{
	std::stringstream ss;
	OArchive oar(ss);

	oar << boost::serialization::make_nvp("t", t);

	ss.flush();

	T tmp;
	IArchive iar(ss);
	iar >> boost::serialization::make_nvp("t", tmp);

	ASSERT_EQ(t, tmp);
}

TEST(Serialization, XML)
{
	using namespace boost::archive;

	test_serialization<xml_oarchive, xml_iarchive>( _int(-42));
	test_serialization<xml_oarchive, xml_iarchive>(_char(-42));
	test_serialization<xml_oarchive, xml_iarchive>(  _ll(-42));
	test_serialization<xml_oarchive, xml_iarchive>(   _d(-42));
}

TEST(Serialization, Binary)
{
	using namespace boost::archive;
	test_serialization<binary_oarchive, binary_iarchive>( _int(-42));
	test_serialization<binary_oarchive, binary_iarchive>(_char(-42));
	test_serialization<binary_oarchive, binary_iarchive>(  _ll(-42));
	test_serialization<binary_oarchive, binary_iarchive>(   _d(-42));
}
