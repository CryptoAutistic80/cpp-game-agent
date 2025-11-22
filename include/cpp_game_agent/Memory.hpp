#pragma once

#include <deque>
#include <string>
#include <nlohmann/json.hpp>

namespace cpp_game_agent {

class Memory {
public:
    explicit Memory(std::size_t limit = 20) : limit_(limit) {}

    void add(const std::string& entry) {
        if (entries_.size() >= limit_) {
            entries_.pop_front();
        }
        entries_.push_back(entry);
    }

    const std::deque<std::string>& entries() const { return entries_; }
    std::size_t limit() const { return limit_; }

private:
    std::deque<std::string> entries_;
    std::size_t limit_;
};

inline void to_json(nlohmann::json& j, const Memory& m) {
    j = nlohmann::json{{"entries", m.entries()}, {"limit", m.limit()}};
}

inline void from_json(const nlohmann::json& j, Memory& m) {
    auto limit = j.value("limit", static_cast<std::size_t>(20));
    Memory temp(limit);
    for (const auto& entry : j.at("entries")) {
        temp.add(entry.get<std::string>());
    }
    m = temp;
}

}  // namespace cpp_game_agent
