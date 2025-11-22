#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace cpp_game_agent {

struct ToolDefinition {
    std::string name;
    std::string description;
    nlohmann::json input_schema = nlohmann::json::object();
    bool use_mcp = false;
    nlohmann::json mcp_spec = nlohmann::json::object();

    static ToolDefinition MCP(const nlohmann::json& server_spec) {
        ToolDefinition tool;
        tool.name = "mcp";
        tool.description = "Model Context Protocol tool bridge";
        tool.use_mcp = true;
        tool.mcp_spec = server_spec;
        return tool;
    }
};

inline void to_json(nlohmann::json& j, const ToolDefinition& t) {
    j = nlohmann::json{{"name", t.name}, {"description", t.description}, {"input_schema", t.input_schema}};
    if (t.use_mcp) {
        j["use_mcp"] = true;
        j["mcp_spec"] = t.mcp_spec;
    }
}

inline void from_json(const nlohmann::json& j, ToolDefinition& t) {
    j.at("name").get_to(t.name);
    j.at("description").get_to(t.description);
    t.input_schema = j.value("input_schema", nlohmann::json::object());
    t.use_mcp = j.value("use_mcp", false);
    t.mcp_spec = j.value("mcp_spec", nlohmann::json::object());
}

}  // namespace cpp_game_agent
