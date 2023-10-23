#include "space_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space {

formats::json::Value Serialize(
	const std::vector<svetit::space::Space> s,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["items"];

	int i = 0;
	for (auto space: s) {
		builder["items"][std::to_string(i)]["id"] = space.id;
		builder["items"][std::to_string(i)]["name"] = space.name;
		builder["items"][std::to_string(i)]["key"] = space.key;
		builder["items"][std::to_string(i)]["requestAllowed"] = space.requestsAllowed;
		builder["items"][std::to_string(i)]["createdAt"] = date::format("%F %T", space.createdAt);
		i++;
    }

	return builder.ExtractValue();
}

} // namespace svetit::space
