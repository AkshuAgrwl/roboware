#include "roboware/core/messages/parser.hpp"

#include <fstream>
#include <iostream>
#include <regex>

namespace rw::core::messages {

inline std::string trim(const std::string &s) {
    auto start = s.find_first_not_of(" \t\r\n");
    auto end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

inline FieldType parse_type_string(const std::string &type_str) {
    // clang-format off
    if      (type_str == "int8")    return FieldType::INT8;
    else if (type_str == "uint8")   return FieldType::UINT8;
    else if (type_str == "int16")   return FieldType::INT16;
    else if (type_str == "uint16")  return FieldType::UINT16;
    else if (type_str == "int32")   return FieldType::INT32;
    else if (type_str == "uint32")  return FieldType::UINT32;
    else if (type_str == "float32") return FieldType::FLOAT32;
    else if (type_str == "float64") return FieldType::FLOAT64;
    else if (type_str == "bool")    return FieldType::BOOL;
    else if (type_str == "string")  return FieldType::STRING;
    else                            return FieldType::UNKNOWN;
    // clang-format on
}

inline Field parse_field_line(const std::string &line) {
    Field field;

    std::string clean_line = line.substr(0, line.find("#"));

    std::regex pattern(
        R"(((\w+)(\[(\d*)\])?)\s+(\w+)\s*(=\s*(\[.*\]|[^#]+))?)");
    std::smatch match;

    if (std::regex_match(clean_line, match, pattern)) {
        field.type = parse_type_string(match[2]);
        field.name = match[5];

        if (match[3].matched) {
            field.is_array = true;

            if (match[4].matched)
                field.array_size = std::stoi(match[4].str());
        }
        if (match[7].matched)
            field.default_val = trim(match[7].str());
    } else {
        std::cerr << "[roboware::core::messages::parser] Invalid field line: "
                  << line << std::endl;
    }

    return field;
}

bool parse_msg(const std::string &path, Message &msg) {
    msg = Message{};
    msg.version = "0.1"; // default version

    std::string ext = path.substr(path.find_last_of('.'));
    if (ext != ".msg" && ext != ".rmsg") {
        std::cerr
            << "[roboware::core::messages::parser] Invalid file extension: "
            << path << std::endl;
        return false;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[roboware::core::messages::parser] Failed to open "
                  << path << std::endl;
        return false;
    }

    std::size_t start = path.find_last_of("/\\");
    std::size_t end = path.find_last_of(".");
    msg.name = path.substr(start + 1, end - start - 1);

    std::string line;
    int lineno = 0;
    bool field_started = false;

    std::regex version_regex(R"(^version\s+(\d+\.\d+)$)");

    while (std::getline(file, line)) {
        lineno++;
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue;

        std::smatch version_match;
        if (std::regex_match(line, version_match, version_regex)) {
            if (field_started) {
                std::cerr
                    << "[roboware::core::messages::parser] "
                       "Version must appear before any field definitions. "
                       "Found at line "
                    << lineno << std::endl;
                return false;
            }
            msg.version = version_match[1];
            continue;
        }

        // Otherwise, treat as field definition
        field_started = true;
        Field field = parse_field_line(line);
        if (field.type == FieldType::UNKNOWN) {
            std::cerr
                << "[roboware::core::messages::parser] Unknown field at line "
                << lineno << ": " << line << std::endl;
        } else {
            msg.fields.push_back(field);
        }
    }

    return true;
}

} // namespace rw::core::messages
