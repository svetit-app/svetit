#include "user_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceUser& su,
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
	return {
		.spaceId = utils::BoostUuidFromString(json["spaceId"].As<std::string>()),
		.userId = json["userId"].As<std::string>(),
		.isOwner = json["isOwner"].As<bool>(),
		.joinedAt = json["joinedAt"].As<int64_t>(),
		.role = Role::FromString(json["role"].As<std::string>())
	};
}

} // namespace svetit::space::model