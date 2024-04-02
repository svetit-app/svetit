#include "measure_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Measure& measure,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = measure.id;
	builder["projectId"] = boost::uuids::to_string(measure.projectId);
	builder["name"] = measure.name;
	builder["isDeleted"] = measure.isDeleted;

	return builder.ExtractValue();
}

Measure Parse(
	const formats::json::Value& json,
	formats::parse::To<Measure>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int>(),
		.projectId = projectId,
		.name = json["name"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model