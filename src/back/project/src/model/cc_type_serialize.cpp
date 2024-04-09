#include "cc_type_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcType& ccType,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = ccType.id;
	builder["projectId"] = boost::uuids::to_string(ccType.projectId);
	builder["key"] = ccType.key;
	builder["name"] = ccType.name;
	builder["description"] = ccType.description;
	builder["isDeleted"] = ccType.isDeleted;

	return builder.ExtractValue();
}

CcType Parse(
	const formats::json::Value& json,
	formats::parse::To<CcType>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int>(),
		.projectId = projectId,
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model