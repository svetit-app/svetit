#include "device_item_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DeviceItem& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["deviceId"] = item.typeId;
	builder["typeId"] = item.typeId;
	builder["name"] = item.name;

	return builder.ExtractValue();
}

DeviceItem Parse(
	const formats::json::Value& json,
	formats::parse::To<DeviceItem>)
{
	return {
		.id = json["id"].As<int64_t>(),
		.deviceId = json["deviceId"].As<int64_t>(),
		.typeId = json["typeId"].As<int64_t>(),
		.name = json["name"].As<std::string>()
	};
}

} // namespace svetit::project::model