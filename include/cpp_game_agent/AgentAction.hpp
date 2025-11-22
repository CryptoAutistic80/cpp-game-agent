#pragma once

#include <optional>
#include <string>
#include <nlohmann/json.hpp>

namespace cpp_game_agent {

struct AgentAction {
    std::string type;
    std::optional<std::string> tool_name;
    nlohmann::json arguments;
};

inline void to_json(nlohmann::json& j, const AgentAction& a) {
    j = nlohmann::json{{"type", a.type}, {"arguments", a.arguments}};
    if (a.tool_name) {
        j["tool_name"] = *a.tool_name;
    }
}

inline void from_json(const nlohmann::json& j, AgentAction& a) {
    j.at("type").get_to(a.type);
    if (j.contains("tool_name")) {
        a.tool_name = j.at("tool_name").get<std::string>();
    }
    a.arguments = j.value("arguments", nlohmann::json::object());
}

}  // namespace cpp_game_agent
