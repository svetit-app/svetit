#include "device_plugin_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DevicePluginParam& devicePluginParam,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["deviceId"] = devicePluginParam.deviceId;
	builder["paramId"] = devicePluginParam.paramId;
	builder["isDeleted"] = devicePluginParam.isDeleted;

	return builder.ExtractValue();
}

DevicePluginParam Parse(
	const formats::json::Value& json,
	formats::parse::To<DevicePluginParam>)
{
	return {
		.deviceId = json["deviceId"].As<int>(),
		.paramId = json["paramId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model