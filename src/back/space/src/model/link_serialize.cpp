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
	builder["spaceId"] = boost::uuids::to_string(sl.spaceId);
	builder["creatorId"] = sl.creatorId;
	builder["name"] = sl.name;
	builder["createdAt"] = std::chrono::duration_cast<std::chrono::seconds>(sl.createdAt.time_since_epoch()).count();
	builder["expiredAt"] = std::chrono::duration_cast<std::chrono::seconds>(sl.expiredAt.time_since_epoch()).count();

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

	const std::chrono::system_clock::time_point createdAt{std::chrono::seconds{json["createdAt"].As<int64_t>()}};
	const std::chrono::system_clock::time_point expiredAt{std::chrono::seconds{json["expiredAt"].As<int64_t>()}};

	return SpaceLink{
		.id = id,
		.spaceId = spaceId,
		.creatorId = json["creatorId"].As<std::string>(""),
		.name = json["name"].As<std::string>(),
		.createdAt = createdAt,
		.expiredAt = expiredAt
	};
}

} // namespace svetit::space::model