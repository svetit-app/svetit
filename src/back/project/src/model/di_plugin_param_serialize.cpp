#include "cc_type_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcTypeParam& ccTypeParam,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["ccTypeId"] = ccTypeParam.ccTypeId;
	builder["paramId"] = ccTypeParam.paramId;
	builder["isDeleted"] = ccTypeParam.isDeleted;

	return builder.ExtractValue();
}

CcTypeParam Parse(
	const formats::json::Value& json,
	formats::parse::To<CcTypeParam>)
{
	return {
		.ccTypeId = json["ccTypeId"].As<int>(),
		.paramId = json["paramId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model