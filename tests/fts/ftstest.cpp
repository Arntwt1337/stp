#include <fts/fts.hpp>

#include <gtest/gtest.h>

TEST(ftp, floatingPointNumbers)
{
	ASSERT_EQ(42, add_floating_point_numbers(40, 2));
}