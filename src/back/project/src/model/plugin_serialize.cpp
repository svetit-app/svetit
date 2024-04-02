#include "plugin_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Plugin& plugin,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = plugin.id;
	builder["projectId"] = boost::uuids::to_string(plugin.projectId);
	builder["name"] = plugin.name;
	builder["description"] = plugin.description;
	builder["key"] = plugin.key;
	builder["isDeleted"] = plugin.isDeleted;

	return builder.ExtractValue();
}

Plugin Parse(
	const formats::json::Value& json,
	formats::parse::To<Plugin>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int>(),
		.projectId = projectId,
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>(),
		.key = json["key"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>(),
	};
}

} // namespace svetit::project::model