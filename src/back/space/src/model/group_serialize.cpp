#include "group_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const Group& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["name"] = item.name;
	builder["description"] = item.description;

	return builder.ExtractValue();
}

Group Parse(
	const formats::json::Value& json,
	formats::parse::To<Group>)
{
	return {
		.id = json["id"].As<int>(0),
		.spaceId = {},
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>()
	};
}

} // namespace svetit::space::model
