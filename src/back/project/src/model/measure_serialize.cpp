#include "measure_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Measure& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["projectId"] = boost::uuids::to_string(item.projectId);
	builder["name"] = item.name;

	return builder.ExtractValue();
}

Measure Parse(
	const formats::json::Value& json,
	formats::parse::To<Measure>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int64_t>(),
		.spaceId = {},
		.projectId = projectId,
		.name = json["name"].As<std::string>()
	};
}

} // namespace svetit::project::model