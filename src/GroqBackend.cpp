#include "cpp_game_agent/GroqBackend.hpp"

#include <curl/curl.h>
#include <stdexcept>
#include <string>
#include <utility>

namespace cpp_game_agent {
namespace {
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* data = static_cast<std::string*>(userp);
    data->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}
}  // namespace

GroqBackend::GroqBackend(GroqConfig config) : config_(std::move(config)) {}

nlohmann::json GroqBackend::prepare_tools(const std::vector<ToolDefinition>& tools) const {
    nlohmann::json tool_array = nlohmann::json::array();
    for (const auto& tool : tools) {
        nlohmann::json tool_json;
        to_json(tool_json, tool);
        tool_array.push_back({{"type", "function"}, {"function", tool_json}});
    }
    return tool_array;
}

nlohmann::json GroqBackend::create_payload(const std::string& prompt, const std::vector<ToolDefinition>& tools) const {
    nlohmann::json payload;
    payload["model"] = config_.model;
    payload["input"] = prompt;
    payload["temperature"] = config_.temperature;
    payload["max_output_tokens"] = config_.max_output_tokens;

    if (!tools.empty()) {
        payload["tools"] = prepare_tools(tools);
    }

    return payload;
}

nlohmann::json GroqBackend::send_request(const std::string& prompt, const std::vector<ToolDefinition>& tools) const {
    nlohmann::json payload = create_payload(prompt, tools);
    const std::string json_body = payload.dump();

    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize libcurl");
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + config_.api_key).c_str());
    headers = curl_slist_append(headers, ("User-Agent: " + config_.user_agent).c_str());

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, config_.endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        throw std::runtime_error(std::string("Failed to call Groq API: ") + curl_easy_strerror(res));
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (http_code >= 400) {
        throw std::runtime_error("Groq API returned HTTP status " + std::to_string(http_code) + ": " + response_data);
    }

    return nlohmann::json::parse(response_data);
}

}  // namespace cpp_game_agent
