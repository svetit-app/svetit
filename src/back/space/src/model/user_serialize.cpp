#include "user_serialize.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceUser& su,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["userId"] = su.userId;
	builder["isOwner"] = su.isOwner;
	builder["joinedAt"] = su.joinedAt;
	builder["roleId"] = su.roleId;

	return builder.ExtractValue();
}

SpaceUser Parse(
	const formats::json::Value& json,
	formats::parse::To<SpaceUser>)
{
	const auto spaceIdStr = json["spaceId"].As<std::string>();
	const auto spaceId = spaceIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(spaceIdStr);

	return {
		.spaceId = spaceId,
		.userId = json["userId"].As<std::string>(""),
		.isOwner = json["isOwner"].As<bool>(),
		.joinedAt = json["joinedAt"].As<int64_t>(),
		.roleId = json["roleId"].As<int>()
	};
}

} // namespace svetit::space::model
