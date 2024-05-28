#include "di_type_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DiType& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["projectId"] = boost::uuids::to_string(item.projectId);
	builder["measureId"] = item.measureId;;
	builder["saveTimerId"] = item.saveTimerId;
	builder["key"] = item.key;
	builder["name"] = item.name;
	builder["mode"] = DiMode::ToString(item.mode);
	builder["saveAlgorithm"] = SaveAlgorithm::ToString(item.saveAlgorithm);

	return builder.ExtractValue();
}

DiType Parse(
	const formats::json::Value& json,
	formats::parse::To<DiType>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int64_t>(),
		.spaceId = {},
		.projectId = projectId,
		.measureId = json["measureId"].As<int64_t>(),
		.saveTimerId = json["saveTimerId"].As<int64_t>(),
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.mode = DiMode::FromString(json["mode"].As<std::string>()),
		.saveAlgorithm = SaveAlgorithm::FromString(json["saveAlgorithm"].As<std::string>())
	};
}

} // namespace svetit::project::model