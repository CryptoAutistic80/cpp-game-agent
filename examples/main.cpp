#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "cpp_game_agent/Agent.hpp"

using namespace cpp_game_agent;

int main() {
    const char* api_key_env = std::getenv("GROQ_API_KEY");
    if (!api_key_env) {
        std::cerr << "GROQ_API_KEY is not set. Requests will fail against the live Groq API.\n";
    }

    std::ifstream persona_file("characters/persona.json");
    std::ifstream behaviour_file("characters/behaviour.json");
    if (!persona_file || !behaviour_file) {
        std::cerr << "Unable to open persona or behaviour files.\n";
        return 1;
    }

    nlohmann::json persona_json;
    nlohmann::json behaviour_json;
    persona_file >> persona_json;
    behaviour_file >> behaviour_json;

    Persona persona = persona_json.get<Persona>();
    Behaviour behaviour = behaviour_json.get<Behaviour>();

    Memory memory(8);
    GroqConfig config;
    if (api_key_env) {
        config.api_key = api_key_env;
    }
    GroqBackend backend(config);

    Agent agent(persona, behaviour, memory, backend);

    std::vector<ToolDefinition> tools;
    tools.push_back({
        "echo",
        "Echoes back the provided phrase for debugging.",
        {{"type", "object"}, {"properties", {{"phrase", {{"type", "string"}}}}}, {"required", {"phrase"}}},
    });

    std::string user_message = "Describe the current mission and propose the first move.";
    std::cout << "Sending prompt...\n";

    try {
        AgentReply reply = agent.act(user_message, tools);
        std::cout << "Agent reply: " << reply.text << "\n";
        if (!reply.actions.empty()) {
            std::cout << "Proposed tool calls:\n";
            for (const auto& action : reply.actions) {
                std::cout << " - " << (action.tool_name.value_or("unknown")) << ": " << action.arguments.dump() << "\n";
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Failed to reach Groq Responses API: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
