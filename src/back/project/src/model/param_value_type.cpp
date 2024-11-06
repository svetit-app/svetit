#include "param_value_type.hpp"
#include <svetit/errors.hpp>

namespace svetit::project {

/*static*/ std::string ParamValueType::ToString(const ParamValueType::Type& paramValueType)
{
	switch (paramValueType) {
	case Int:
		return "int";
	case Bool:
		return "bool";
	case Float:
		return "float";
	case String:
		return "string";
	case Bytes:
		return "bytes";
	case Time:
		return "time";
	case Range:
		return "range";
	case Combo:
		return "combo";
	default:
		break;
	}

	return {};
}

/*static*/ ParamValueType::Type ParamValueType::FromString(const std::string& paramValueType)
{
	if (paramValueType == "int")
		return Int;
	if (paramValueType == "bool")
		return Bool;
	if (paramValueType == "float")
		return Float;
	if (paramValueType == "string")
		return String;
	if (paramValueType == "bytes")
		return Bytes;
	if (paramValueType == "time")
		return Time;
	if (paramValueType == "range")
		return Range;
	if (paramValueType == "combo")
		return Combo;

	throw errors::BadRequest400("Wrong param value type");
}

formats::json::Value Serialize(
	const ParamValueType::Type& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder res{ParamValueType::ToString(item)};
	return res.ExtractValue();
}

ParamValueType::Type Parse(
	const formats::json::Value& json,
	formats::parse::To<ParamValueType::Type>)
{
	return ParamValueType::FromString(json.As<std::string>());
}

} // namespace svetit::project
