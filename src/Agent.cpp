#include "cpp_game_agent/Agent.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace cpp_game_agent {

Agent::Agent(Persona persona, Behaviour behaviour, Memory memory, GroqBackend backend)
    : persona_(std::move(persona)),
      behaviour_(std::move(behaviour)),
      memory_(std::move(memory)),
      backend_(std::move(backend)) {}

std::string Agent::build_prompt(const std::string& user_message) const {
    std::ostringstream prompt;
    prompt << "You are " << persona_.name << " (" << persona_.role << ").\n";
    prompt << persona_.description << "\n\n";

    prompt << "Behaviour:\n";
    prompt << "- Speaking style: " << behaviour_.speaking_style << "\n";
    for (const auto& directive : behaviour_.directives) {
        prompt << "- " << directive << "\n";
    }

    if (!memory_.entries().empty()) {
        prompt << "\nRecent memory:\n";
        for (const auto& entry : memory_.entries()) {
            prompt << "- " << entry << "\n";
        }
    }

    prompt << "\nUser: " << user_message << "\nAssistant:";
    return prompt.str();
}

AgentReply Agent::act(const std::string& user_message, const std::vector<ToolDefinition>& tools) {
    memory_.add("User: " + user_message);
    const std::string prompt = build_prompt(user_message);

    const auto response_json = backend_.send_request(prompt, tools);
    AgentReply reply = parse_response(response_json);

    memory_.add("Assistant: " + reply.text);
    return reply;
}

AgentReply Agent::parse_response(const nlohmann::json& response_json) const {
    AgentReply reply;

    if (response_json.contains("output_text")) {
        reply.text = response_json.at("output_text").get<std::string>();
    } else if (response_json.contains("response") && response_json.at("response").contains("output_text")) {
        reply.text = response_json.at("response").at("output_text").get<std::string>();
    }

    if (reply.text.empty() && response_json.contains("choices")) {
        const auto& choice = response_json.at("choices").front();
        const auto& message = choice.at("message");
        if (message.contains("content")) {
            // If content is an array, concatenate text values.
            if (message.at("content").is_array()) {
                for (const auto& part : message.at("content")) {
                    if (part.contains("text")) {
                        reply.text += part.at("text").get<std::string>();
                    }
                }
            } else {
                reply.text = message.at("content").get<std::string>();
            }
        }
        reply.actions = parse_tool_calls(message);
    }

    // Fallback to a generic message if still empty.
    if (reply.text.empty() && response_json.contains("message")) {
        reply.text = response_json.at("message").get<std::string>();
    }

    return reply;
}

std::vector<AgentAction> Agent::parse_tool_calls(const nlohmann::json& message_json) const {
    std::vector<AgentAction> actions;
    if (!message_json.contains("tool_calls")) {
        return actions;
    }

    for (const auto& call : message_json.at("tool_calls")) {
        AgentAction action;
        action.type = call.value("type", "tool_call");

        if (action.type == "mcp" && call.contains("mcp")) {
            if (call.contains("id")) {
                action.tool_name = call.at("id").get<std::string>();
            }
            action.arguments = call.at("mcp");
            actions.push_back(action);
            continue;
        }

        if (call.contains("function")) {
            const auto& func = call.at("function");
            if (func.contains("name")) {
                action.tool_name = func.at("name").get<std::string>();
            }
            if (func.contains("arguments")) {
                try {
                    const auto& args_raw = func.at("arguments");
                    if (args_raw.is_string()) {
                        action.arguments = nlohmann::json::parse(args_raw.get<std::string>());
                    } else {
                        action.arguments = args_raw;
                    }
                } catch (const std::exception&) {
                    action.arguments = nlohmann::json::object();
                }
            }
        }
        actions.push_back(action);
    }

    return actions;
}

}  // namespace cpp_game_agent
