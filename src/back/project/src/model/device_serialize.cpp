#include "device_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Device& device,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = device.id;
	builder["projectId"] = boost::uuids::to_string(device.projectId);
	builder["pluginId"] = device.pluginId;
	builder["name"] = device.name;
	builder["check_interval_msec"] = device.check_interval_msec;
	builder["isDeleted"] = device.isDeleted;

	return builder.ExtractValue();
}

Device Parse(
	const formats::json::Value& json,
	formats::parse::To<Device>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int>(),
		.projectId = projectId,
		.pluginId = json["pluginId"].As<int>(),
		.name = json["name"].As<std::string>(),
		.check_interval_msec = json["check_interval_msec"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>(),
	};
}

} // namespace svetit::project::model