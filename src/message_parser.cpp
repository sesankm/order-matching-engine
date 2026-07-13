#include "message_parser.hpp"

MessageParser::MessageParser() {
};

bool MessageParser::compare_string(const std::string_view a, const std::string_view b) {
    return std::ranges::equal(a.begin(), a.end(), b.begin(), b.end(),
                              [](const char a1, const char b1) {
                                  return std::toupper(a1) == b1;
                              });
}

Order MessageParser::parse(const std::string &message) {
    std::vector<std::string> args{};
    std::stringstream ss{message};
    std::string arg;

    while (ss >> arg) { args.push_back(arg); }
    if (validate_args(args)) {
        return {
            std::strtof(args[2].c_str(), nullptr),     // PRICE
            std::strtol(args[3].c_str(), nullptr, 10), // QUANTITY
            compare_string(args[1], LIMIT) ? OType::LIMIT : OType::MARKET,
            compare_string(args[0], BUY_OP) ? OSide::BUY : OSide::SELL
        };
    }
    throw std::runtime_error{"Invalid message received."};
}

bool MessageParser::validate_args(const std::vector<std::string>& args){
    return args.size() == 4;
}
