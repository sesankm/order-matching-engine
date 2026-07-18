#include "ring_buffer.hpp"
#include "gtest/gtest.h"
#include <optional>
#include <string>


TEST(RingBufferTest, testReadFailsOnEmpty) {
    RingBuffer ringBuffer {3};
    EXPECT_FALSE(ringBuffer.read());
}

TEST(RingBufferTest, testWritesIgnoredOnFull) {
    RingBuffer ringBuffer {3};

    ringBuffer.write("foo");
    ringBuffer.write("bar");
    ringBuffer.write("baz");
    ringBuffer.write("invalid");
    ringBuffer.write("invalid2");

    EXPECT_EQ(ringBuffer.read().value(), "foo");
    EXPECT_EQ(ringBuffer.read().value(), "bar");
    EXPECT_EQ(ringBuffer.read().value(), "baz");
    EXPECT_EQ(ringBuffer.read().has_value(), false);
    EXPECT_EQ(ringBuffer.read().has_value(), false);
}

TEST(RingBufferTest, testReadWriteOrder) {
    RingBuffer ringBuffer {3};

    ringBuffer.write("foo");
    ringBuffer.write("bar");
    ringBuffer.write("baz");
    
    EXPECT_EQ(ringBuffer.read().value(), "foo");
    EXPECT_EQ(ringBuffer.read().value(), "bar");

    ringBuffer.write("foo");
    ringBuffer.write("bar");

    EXPECT_EQ(ringBuffer.read().value(), "baz");
    EXPECT_EQ(ringBuffer.read().value(), "foo");
    EXPECT_EQ(ringBuffer.read().value(), "bar");
}

TEST(RingBufferTest, testContinuousWrapAround) {
    RingBuffer rb{3};
    
    // Cycle 1
    rb.write("1"); rb.write("2");
    EXPECT_EQ(rb.read().value(), "1");
    
    // Cycle 2
    rb.write("3");
    EXPECT_EQ(rb.read().value(), "2");
    EXPECT_EQ(rb.read().value(), "3");
    
    // Cycle 3
    rb.write("4");
    EXPECT_EQ(rb.read().value(), "4");
}
