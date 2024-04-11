#include "cc_mode_type_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcModeType& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["ccTypeId"] = item.ccTypeId;
	builder["key"] = item.key;
	builder["name"] = item.name;
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

	return builder.ExtractValue();
}

CcModeType Parse(
	const formats::json::Value& json,
	formats::parse::To<CcModeType>)
{
	return {
		.id = json["id"].As<int>(),
		.ccTypeId = json["ccTypeId"].As<int>(),
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model