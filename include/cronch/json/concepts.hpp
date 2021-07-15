
#pragma once 

#include <nlohmann/json.hpp>

namespace cronch::json::concepts {

template<typename J, typename BasicJsonType = nlohmann::json>
concept json_serializable = requires(const J& v, BasicJsonType& j)
{
    BasicJsonType::template json_serializer<J, std::void_t<J>>::to_json(j, v);
};

template<typename J, typename BasicJsonType = nlohmann::json>
concept json_deserializable = requires(J& v, const BasicJsonType& j) {
    BasicJsonType::template json_serializer<J, std::void_t<J>>::from_json(j, v);
};

static_assert(json_deserializable<int>, "json_derserializable is incorrect, this is a bug");
static_assert(json_serializable<int>, "json_serializable is incorrect, this is a bug");
} // namespace cronch::json::concepts
