#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <iostream>
#include <string>
#include "order.hpp"
#include <ranges>
#include <sstream>

constexpr std::string BUY_OP  = "BUY";
constexpr std::string SELL_OP = "SELL";
constexpr std::string LIMIT  = "LIM";
constexpr std::string MARKET = "MKT";

class MessageParser {
public:
    MessageParser();
    static bool validate_args(const std::vector<std::string>& args);
    static bool compare_string(const std::string_view a, const std::string_view b);
    static Order parse(const std::string& message);
};


#endif
