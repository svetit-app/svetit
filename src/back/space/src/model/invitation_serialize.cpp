#include "invitation_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceInvitation& si,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = si.id;
	builder["spaceId"] = boost::uuids::to_string(si.spaceId);
	builder["userId"] = si.userId;
	builder["role"] = Role::ToString(si.role);
	builder["creatorId"] = si.creatorId;
	builder["createdAt"] = si.createdAt;

	return builder.ExtractValue();
}

model::SpaceInvitation Parse(
	const formats::json::Value& json,
	formats::parse::To<model::SpaceInvitation>)
{
	return {
		.id = json["id"].As<int>(),
		.spaceId= utils::BoostUuidFromString(json["spaceId"].As<std::string>()),
		.creatorId = json["creatorId"].As<std::string>(),
		.userId = json["userId"].As<std::string>(),
		.role = Role::FromString(json["role"].As<std::string>()),
		.createdAt = json["createdAt"].As<int64_t>(),
	};
}

} // namespace svetit::space::model