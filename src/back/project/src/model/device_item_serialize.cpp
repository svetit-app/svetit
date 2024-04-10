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
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

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