#include "param_type_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ParamType& paramType,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = paramType.id;
	builder["parentId"] = paramType.parentId;;
	builder["key"] = paramType.key;
	builder["name"] = paramType.name;
	builder["description"] = paramType.description;
	builder["valueType"] = ParamValueType::ToString(paramType.valueType);
	builder["isDeleted"] = paramType.isDeleted;

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