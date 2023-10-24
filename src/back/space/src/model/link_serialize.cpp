#include "link_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceLink sl,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = sl.id;
	builder["spaceId"] = sl.spaceId;
	builder["creatorId"] = sl.creatorId;
	builder["name"] = sl.name;
	builder["createdAt"] = date::format("%F %T", sl.createdAt);
	builder["expiredAt"] = date::format("%F %T", sl.expiredAt);

	return builder.ExtractValue();
}

} // namespace svetit::space::model
