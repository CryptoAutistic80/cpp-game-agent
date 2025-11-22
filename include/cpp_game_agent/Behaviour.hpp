#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace cpp_game_agent {

struct Behaviour {
    std::string speaking_style;
    std::vector<std::string> directives;

    Behaviour() = default;
    Behaviour(std::string style, std::vector<std::string> directives_)
        : speaking_style(std::move(style)), directives(std::move(directives_)) {}
};

inline void to_json(nlohmann::json& j, const Behaviour& b) {
    j = nlohmann::json{{"speaking_style", b.speaking_style}, {"directives", b.directives}};
}

inline void from_json(const nlohmann::json& j, Behaviour& b) {
    j.at("speaking_style").get_to(b.speaking_style);
    j.at("directives").get_to(b.directives);
}

}  // namespace cpp_game_agent
