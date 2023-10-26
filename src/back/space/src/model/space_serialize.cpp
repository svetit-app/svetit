#include "space_serialize.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::Space s,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = boost::uuids::to_string(s.id);
	builder["name"] = s.name;
	builder["key"] = s.key;
	builder["requestAllowed"] = s.requestsAllowed;
	builder["createdAt"] = date::format("%F %T", s.createdAt);

	return builder.ExtractValue();
}

} // namespace svetit::space::model
