#include "di_plugin_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DiPluginParam& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["diTypeId"] = item.diTypeId;
	builder["paramId"] = item.paramId;
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

	return builder.ExtractValue();
}

DiPluginParam Parse(
	const formats::json::Value& json,
	formats::parse::To<DiPluginParam>)
{
	return {
		.diTypeId = json["diTypeId"].As<int>(),
		.paramId = json["paramId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model