#include "section_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Section& section,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = section.id;
	builder["projectId"] = boost::uuids::to_string(section.projectId);
	builder["name"] = section.name;
	builder["isDeleted"] = section.isDeleted;

	return builder.ExtractValue();
}

Section Parse(
	const formats::json::Value& json,
	formats::parse::To<Section>)
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