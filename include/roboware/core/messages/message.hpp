#pragma once

#include <optional>
#include <string>
#include <vector>

namespace rw::core::messages {

enum class FieldType {
    // clang-format off
    INT8, UINT8,
    INT16, UINT16,
    INT32, UINT32,
    FLOAT32, FLOAT64,
    BOOL,
    STRING,
    UNKNOWN,
    // clang-format on
};

struct Field {
    std::string name;
    FieldType type;
    bool is_array = false;
    int array_size = -1; // -1 for dynamic
    std::optional<std::string> default_val;
};

struct Message {
    std::string name;
    std::string version = "0.1";
    std::vector<Field> fields;
};

} // namespace rw::core::messages
