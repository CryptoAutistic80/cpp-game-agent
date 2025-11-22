#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "ToolDefinition.hpp"

namespace cpp_game_agent {

struct GroqConfig {
    std::string api_key;
    std::string model = "llama-3.1-70b-versatile";
    double temperature = 0.7;
    int max_output_tokens = 512;
    std::string endpoint = "https://api.groq.com/openai/v1/responses";
    std::string user_agent = "cpp-game-agent/0.1";
};

class GroqBackend {
public:
    explicit GroqBackend(GroqConfig config);

    nlohmann::json create_payload(const std::string& prompt, const std::vector<ToolDefinition>& tools = {}) const;

    nlohmann::json send_request(const std::string& prompt, const std::vector<ToolDefinition>& tools = {}) const;

private:
    GroqConfig config_;
    nlohmann::json prepare_tools(const std::vector<ToolDefinition>& tools) const;
};

}  // namespace cpp_game_agent
