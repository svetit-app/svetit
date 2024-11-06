#include "node_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const Node& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = boost::uuids::to_string(item.id);
	builder["spaceId"] = boost::uuids::to_string(item.spaceId);
	builder["name"] = item.name;
	builder["description"] = item.description;
	builder["latitude"] = item.latitude;
	builder["longitude"] = item.longitude;
	builder["createdAt"] = item.createdAt;

	return builder.ExtractValue();
}

Node Parse(
	const formats::json::Value& json,
	formats::parse::To<Node>)
{
	const auto idStr = json["id"].As<std::string>("");
	const auto id = idStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(idStr);

	return {
		.id = id,
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>(),
		.latitude = json["latitude"].As<double>(),
		.longitude = json["longitude"].As<double>(),
		.createdAt = json["createdAt"].As<int64_t>(0)
	};
}

} // namespace svetit::node::model
