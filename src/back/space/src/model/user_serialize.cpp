#include "user_serialize.hpp"

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceUser& su,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["spaceId"] = boost::uuids::to_string(su.spaceId);
	builder["userId"] = su.userId;
	builder["isOwner"] = su.isOwner;
	builder["joinedAt"] = su.joinedAt;
	builder["role"] = Role::ToString(su.role);

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
		.userId = json["userId"].As<std::string>(),
		.isOwner = json["isOwner"].As<bool>(),
		.joinedAt = json["joinedAt"].As<int64_t>(),
		.role = Role::FromString(json["role"].As<std::string>())
	};
}

} // namespace svetit::space::model