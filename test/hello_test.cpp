#include <gtest/gtest.h>

#include "../src/cmp/Led.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
	Component::Led l({ Driver::EPort::PORT_B, Driver::EPin::PIN_0 });
	// Expect two strings not to be equal.
	EXPECT_STRNE("world", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}
