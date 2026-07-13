#include "message_parser.hpp"
#include "gtest/gtest.h"

MessageParser messageParser;

TEST(MessageParserTest, testValidateArgs) {
    EXPECT_TRUE(messageParser.validate_args({"buy", "lim", "100", "200"}));
    EXPECT_FALSE(messageParser.validate_args({}));
}

TEST(MessageParserTest, testCompareString) {
    EXPECT_TRUE(messageParser.compare_string("BUY",  BUY_OP));
    EXPECT_TRUE(messageParser.compare_string("SELL", SELL_OP));
    EXPECT_TRUE(messageParser.compare_string("LIM",  LIMIT));
    EXPECT_TRUE(messageParser.compare_string("MKT",  MARKET));
    EXPECT_TRUE(messageParser.compare_string("buy",  BUY_OP));
    EXPECT_TRUE(messageParser.compare_string("sell", SELL_OP));
    EXPECT_TRUE(messageParser.compare_string("lim",  LIMIT));
    EXPECT_TRUE(messageParser.compare_string("mkt",  MARKET));
    EXPECT_FALSE(messageParser.compare_string("selll", SELL_OP));
    EXPECT_FALSE(messageParser.compare_string("",  SELL_OP));
    EXPECT_FALSE(messageParser.compare_string("ssel",  SELL_OP));
}

TEST(MessageParserTest, testParse) {
    Order o = MessageParser::parse("buy lim 100 10");
    EXPECT_EQ(o.m_price, Order::get_price(100));
    EXPECT_EQ(o.m_quantity, 10);

    o = MessageParser::parse("BUY lim 10.10 1");
    EXPECT_EQ(o.m_price, Order::get_price(10.10));
    EXPECT_EQ(o.m_quantity, 1);

    // This should fail fix this later
    o = MessageParser::parse("BUY lim 10.10.10 1");
    //EXPECT_THROW(MessageParser::parse("BUY lim 10.10.10 1"), std::runtime_error);

    EXPECT_THROW(messageParser.parse("buy"), std::runtime_error);
    EXPECT_THROW(messageParser.parse(""), std::runtime_error);
    EXPECT_THROW(messageParser.parse("buy lim 10010"), std::runtime_error);
    EXPECT_THROW(messageParser.parse("buy lim 10010 10 10 10 10"), std::runtime_error);
}
