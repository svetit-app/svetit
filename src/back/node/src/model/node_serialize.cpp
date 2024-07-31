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
	builder["name"] = item.name;
	builder["description"] = item.description;
	builder["latitude"] = item.latitude;
	builder["longitude"] = item.longitude;
	builder["createdAt"] = std::chrono::duration_cast<std::chrono::seconds>(item.createdAt.time_since_epoch()).count();

	return builder.ExtractValue();
}

Node Parse(
	const formats::json::Value& json,
	formats::parse::To<Node>)
{
	const auto idStr = json["id"].As<std::string>("");
	const auto id = idStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(idStr);

	const std::chrono::system_clock::time_point createdAt{std::chrono::seconds{json["createdAt"].As<int64_t>(0)}};

	return {
		.id = id,
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>(),
		.latitude = json["latitude"].As<double>(),
		.longitude = json["longitude"].As<double>(),
		.createdAt = createdAt
	};
}

} // namespace svetit::project::model