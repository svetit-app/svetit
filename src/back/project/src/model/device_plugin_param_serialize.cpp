#include "device_plugin_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DevicePluginParam& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["deviceId"] = item.deviceId;
	builder["paramId"] = item.paramId;

	return builder.ExtractValue();
}

DevicePluginParam Parse(
	const formats::json::Value& json,
	formats::parse::To<DevicePluginParam>)
{
	return {
		.spaceId = {},
		.deviceId = json["deviceId"].As<int64_t>(),
		.paramId = json["paramId"].As<int64_t>()
	};
}

} // namespace svetit::project::model