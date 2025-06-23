#pragma once

#include "message.hpp"

namespace rw::core::messages {

std::string trim(const std::string &s);

FieldType parse_type_string(const std::string &type_str);
Field parse_field_line(const std::string &line);
bool parse_msg(const std::string raw, const std::string msg_name, Message &msg);

} // namespace rw::core::messages
