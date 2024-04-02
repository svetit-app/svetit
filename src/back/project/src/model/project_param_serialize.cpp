#include "project_param_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ProjectParam& projectParam,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["projectId"] = boost::uuids::to_string(projectParam.projectId);
	builder["paramId"] = projectParam.paramId;
	builder["isDeleted"] = projectParam.isDeleted;

	return builder.ExtractValue();
}

ProjectParam Parse(
	const formats::json::Value& json,
	formats::parse::To<ProjectParam>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.projectId = projectId,
		.paramId = json["paramId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model