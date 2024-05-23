#include "cc_mode_type_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcModeType& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["projectId"] = boost::uuids::to_string(item.projectId);
	builder["ccTypeId"] = item.ccTypeId;
	builder["key"] = item.key;
	builder["name"] = item.name;

	return builder.ExtractValue();
}

CcModeType Parse(
	const formats::json::Value& json,
	formats::parse::To<CcModeType>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int64_t>(),
		.projectId = projectId,
		.ccTypeId = json["ccTypeId"].As<int64_t>(),
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>()
	};
}

} // namespace svetit::project::model