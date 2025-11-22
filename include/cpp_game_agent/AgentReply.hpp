#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "AgentAction.hpp"

namespace cpp_game_agent {

struct AgentReply {
    std::string text;
    std::vector<AgentAction> actions;
};

inline void to_json(nlohmann::json& j, const AgentReply& r) {
    j = nlohmann::json{{"text", r.text}, {"actions", r.actions}};
}

inline void from_json(const nlohmann::json& j, AgentReply& r) {
    r.text = j.value("text", std::string{});
    r.actions = j.value("actions", std::vector<AgentAction>{});
}

}  // namespace cpp_game_agent
