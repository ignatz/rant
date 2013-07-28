// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#include <gtest/gtest.h>
#include <unordered_set>

#include "test/random.h"

TEST(Random, Basic)
{
	std::unordered_set<int> ht;

	size_t const N = 10000;

	// during insertion, the hash table will resize itself, such that
	// most bins won't be filled with more than 0 .. 2 entries.
	for (size_t ii=0; ii<N; ++ii)
		ht.insert(random<int>());

	// this will fail, if the the rng generates to many keys with the same hash
	// (most likely same keys)
	EXPECT_LT(N/15, ht.bucket_count());
	ASSERT_GE(N, ht.size());
	ASSERT_LT(1u, ht.size());
}
