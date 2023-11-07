#include "user_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceUser su,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["spaceId"] = boost::uuids::to_string(su.spaceId);
	builder["userId"] = boost::uuids::to_string(su.userId);
	builder["isOwner"] = su.isOwner;
	builder["joinedAt"] = date::format("%F %T", su.joinedAt);
	builder["role"] = su.role;

	return builder.ExtractValue();
}

} // namespace svetit::space::model