#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace cpp_game_agent {

struct ToolDefinition {
    std::string name;
    std::string description;
    nlohmann::json input_schema;
};

inline void to_json(nlohmann::json& j, const ToolDefinition& t) {
    j = nlohmann::json{{"name", t.name}, {"description", t.description}, {"input_schema", t.input_schema}};
}

inline void from_json(const nlohmann::json& j, ToolDefinition& t) {
    j.at("name").get_to(t.name);
    j.at("description").get_to(t.description);
    t.input_schema = j.value("input_schema", nlohmann::json::object());
}

}  // namespace cpp_game_agent
