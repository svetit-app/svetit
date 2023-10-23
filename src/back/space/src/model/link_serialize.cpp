#include "link_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space {

formats::json::Value Serialize(
	const std::vector<svetit::space::SpaceLink> sl,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["items"];

	int i = 0;
	for (auto l: sl) {
		builder["items"][std::to_string(i)]["id"] = l.id;
		builder["items"][std::to_string(i)]["spaceId"] = l.spaceId;
		builder["items"][std::to_string(i)]["creatorId"] = l.creatorId;
		builder["items"][std::to_string(i)]["name"] = l.name;
		builder["items"][std::to_string(i)]["createdAt"] = date::format("%F %T", l.createdAt);
		builder["items"][std::to_string(i)]["expiredAt"] = date::format("%F %T", l.expiredAt);
		i++;
    }

	return builder.ExtractValue();
}

} // namespace svetit::space
