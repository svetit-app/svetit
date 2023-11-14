#include "link_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

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
	builder["createdAt"] = sl.createdAt;
	builder["expiredAt"] = sl.expiredAt;

	return builder.ExtractValue();
}

} // namespace svetit::space::model