#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "AgentReply.hpp"
#include "Behaviour.hpp"
#include "GroqBackend.hpp"
#include "Memory.hpp"
#include "Persona.hpp"
#include "ToolDefinition.hpp"

namespace cpp_game_agent {

class Agent {
public:
    Agent(Persona persona, Behaviour behaviour, Memory memory, GroqBackend backend);

    std::string build_prompt(const std::string& user_message) const;

    AgentReply act(const std::string& user_message, const std::vector<ToolDefinition>& tools = {});

private:
    Persona persona_;
    Behaviour behaviour_;
    Memory memory_;
    GroqBackend backend_;

    AgentReply parse_response(const nlohmann::json& response_json) const;
    std::vector<AgentAction> parse_tool_calls(const nlohmann::json& message_json) const;
};

}  // namespace cpp_game_agent
