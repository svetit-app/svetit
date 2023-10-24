#include "invitation_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceInvitation si,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = si.id;
	builder["spaceId"] = si.spaceId;
	builder["creatorId"] = si.creatorId;
	builder["userId"] = si.userId;
	builder["role"] = si.role;
	builder["createdAt"] = date::format("%F %T", si.createdAt);

	return builder.ExtractValue();
}

} // namespace svetit::space::model
