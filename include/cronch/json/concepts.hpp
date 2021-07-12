
#pragma once 

#include <nlohmann/json.hpp>

namespace cronch::json::concepts {

template<typename J>
concept json_serializable = requires(const J& v, nlohmann::json& j)
{
    j.emplace_back(v);
};

template<typename J>
concept json_deserializable = std::constructible_from<nlohmann::json, J>;
} // namespace cronch::json::concepts
