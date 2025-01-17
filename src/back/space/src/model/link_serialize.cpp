#include "link_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceLink& sl,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = boost::uuids::to_string(sl.id);
	builder["creatorId"] = sl.creatorId;
	builder["name"] = sl.name;
	builder["createdAt"] = sl.createdAt;
	builder["expiredAt"] = sl.expiredAt;

	return builder.ExtractValue();
}

SpaceLink Parse(
	const formats::json::Value& json,
	formats::parse::To<SpaceLink>)
{
	const auto idStr = json["id"].As<std::string>("");
	const auto id = idStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(idStr);

	const auto spaceIdStr = json["spaceId"].As<std::string>();
	const auto spaceId = spaceIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(spaceIdStr);

	return SpaceLink{
		.id = id,
		.spaceId = spaceId,
		.creatorId = json["creatorId"].As<std::string>(""),
		.name = json["name"].As<std::string>(),
		.createdAt = json["createdAt"].As<int64_t>(0),
		.expiredAt = json["expiredAt"].As<int64_t>()
	};
}

} // namespace svetit::space::model
