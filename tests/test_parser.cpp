#include "roboware/core/messages/parser.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fstream>

using namespace rw::core::messages;

bool read_msg_file(const std::string path, std::string &raw,
                   std::string &name) {
    std::size_t start = path.find_last_of("/\\");
    std::size_t end = path.find_last_of(".");
    name = path.substr(start + 1, end - start - 1);

    std::ifstream file(path);
    if (!file)
        return false;
    raw = std::string(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>());

    return true;
}

// TEST_CASE("Parsing a valid .msg file") {
//     Message msg;
//     std::string raw, name;
//     REQUIRE(read_msg_file("data/test_parser.msg", raw, name) == true);
//     REQUIRE(parse_msg(raw, name, msg) == true);
//     REQUIRE(msg.name == "test_parser");
//     REQUIRE(msg.fields.size() == 2);
//     REQUIRE(msg.fields[0].name == "position");
//     REQUIRE(msg.fields[0].type == FieldType::FLOAT32);
// }

TEST_CASE("Parsing a valid .rmsg file") {
    Message msg;
    std::string raw, name;
    REQUIRE(read_msg_file("data/test_parser.rmsg", raw, name) == true);
    REQUIRE(parse_msg(raw, name, msg) == true);
    REQUIRE(msg.name == "test_parser");
    REQUIRE(msg.fields.size() == 1);
    REQUIRE(msg.fields[0].name == "velocity");
    REQUIRE(msg.fields[0].type == FieldType::STRING);
}

// TEST_CASE("Invalid file extension is rejected") {
//     Message msg;
//     std::string raw, name;
//     REQUIRE(read_msg_file("data/test_parser.txt", raw, name) == true);
//     REQUIRE(parse_msg(raw, name, msg) == false);
// }

TEST_CASE("Version definition after any fields is rejected") {
    Message msg;
    std::string raw, name;
    REQUIRE(read_msg_file("data/test_parser_version_1.rmsg", raw, name) ==
            true);
    REQUIRE(parse_msg(raw, name, msg) == false);
}

TEST_CASE("Version check") {
    Message msg;
    std::string raw, name;
    REQUIRE(read_msg_file("data/test_parser_version_2.rmsg", raw, name) ==
            true);
    REQUIRE(parse_msg(raw, name, msg) == true);
    REQUIRE(msg.version == "1.15");
}

TEST_CASE("message complete validity check") {
    Message msg;
    std::string raw, name;
    REQUIRE(read_msg_file("data/test_parser_complete.rmsg", raw, name) == true);
    REQUIRE(parse_msg(raw, name, msg) == true);

    REQUIRE(msg.name == "test_parser_complete");
    REQUIRE(msg.version == "1.10");
    REQUIRE(msg.fields.size() == 3);

    REQUIRE(msg.fields[0].name == "pos");
    REQUIRE(msg.fields[0].type == FieldType::BOOL);
    REQUIRE(msg.fields[0].is_array == false);
    REQUIRE(msg.fields[0].array_size == -1);
    REQUIRE(msg.fields[0].default_val.value_or("null") == "false");

    REQUIRE(msg.fields[1].name == "vel_set");
    REQUIRE(msg.fields[1].type == FieldType::FLOAT64);
    REQUIRE(msg.fields[1].is_array == true);
    REQUIRE(msg.fields[1].array_size == 3);
    REQUIRE(msg.fields[1].default_val.value_or("null") == "[1.0, 2.5, 3.3]");

    REQUIRE(msg.fields[2].name == "acc_str");
    REQUIRE(msg.fields[2].type == FieldType::STRING);
    REQUIRE(msg.fields[2].is_array == false);
    REQUIRE(msg.fields[2].array_size == -1);
    REQUIRE(msg.fields[2].default_val.value_or("null") == "null");
}
