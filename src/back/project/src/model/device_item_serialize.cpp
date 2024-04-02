#include "device_item_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DeviceItem& deviceItem,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = deviceItem.id;
	builder["deviceId"] = deviceItem.typeId;
	builder["typeId"] = deviceItem.typeId;
	builder["name"] = deviceItem.name;
	builder["isDeleted"] = deviceItem.isDeleted;

	return builder.ExtractValue();
}

DeviceItem Parse(
	const formats::json::Value& json,
	formats::parse::To<DeviceItem>)
{
	return {
		.id = json["id"].As<int>(),
		.deviceId = json["deviceId"].As<int>(),
		.typeId = json["typeId"].As<int>(),
		.name = json["name"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>(),
	};
}

} // namespace svetit::project::model