#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace cpp_game_agent {

struct Persona {
    std::string name;
    std::string description;
    std::string role;

    Persona() = default;
    Persona(std::string name_, std::string description_, std::string role_)
        : name(std::move(name_)), description(std::move(description_)), role(std::move(role_)) {}
};

inline void to_json(nlohmann::json& j, const Persona& p) {
    j = nlohmann::json{{"name", p.name}, {"description", p.description}, {"role", p.role}};
}

inline void from_json(const nlohmann::json& j, Persona& p) {
    j.at("name").get_to(p.name);
    j.at("description").get_to(p.description);
    j.at("role").get_to(p.role);
}

}  // namespace cpp_game_agent
