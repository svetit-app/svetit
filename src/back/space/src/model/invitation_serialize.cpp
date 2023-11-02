#include "invitation_serialize.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

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
	builder["spaceId"] = boost::uuids::to_string(si.spaceId);
	builder["userId"] = boost::uuids::to_string(si.userId);
	builder["role"] = si.role;
	builder["creatorId"] = boost::uuids::to_string(si.creatorId);
	builder["createdAt"] = date::format("%F %T", si.createdAt);

	return builder.ExtractValue();
}

} // namespace svetit::space::model