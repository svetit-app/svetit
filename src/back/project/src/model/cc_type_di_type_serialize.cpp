#include "cc_type_di_type_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcTypeDiType& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["ccTypeId"] = item.ccTypeId;
	builder["diTypeId"] = item.diTypeId;

	return builder.ExtractValue();
}

CcTypeDiType Parse(
	const formats::json::Value& json,
	formats::parse::To<CcTypeDiType>)
{
	return {
		.ccTypeId = json["ccTypeId"].As<int>(),
		.diTypeId = json["diTypeId"].As<int>()
	};
}

} // namespace svetit::project::model