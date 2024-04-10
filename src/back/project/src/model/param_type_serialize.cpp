#include "param_type_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ParamType& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["parentId"] = item.parentId;;
	builder["key"] = item.key;
	builder["name"] = item.name;
	builder["description"] = item.description;
	builder["valueType"] = ParamValueType::ToString(item.valueType);
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

	return builder.ExtractValue();
}

ParamType Parse(
	const formats::json::Value& json,
	formats::parse::To<ParamType>)
{
	return {
		.id = json["id"].As<int>(),
		.parentId = json["parentId"].As<int>(),
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>(),
		.valueType = ParamValueType::FromString(json["valueType"].As<std::string>()),
		.isDeleted = json["isDeleted"].As<bool>(),
	};
}

} // namespace svetit::project::model