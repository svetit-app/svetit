#include "cc_status_category_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcStatusCategory& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["projectId"] = boost::uuids::to_string(item.projectId);
	builder["key"] = item.key;
	builder["name"] = item.name;
	builder["color"] = item.color;

	return builder.ExtractValue();
}

CcStatusCategory Parse(
	const formats::json::Value& json,
	formats::parse::To<CcStatusCategory>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int64_t>(),
		.projectId = projectId,
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.color = json["color"].As<std::string>()
	};
}

} // namespace svetit::project::model