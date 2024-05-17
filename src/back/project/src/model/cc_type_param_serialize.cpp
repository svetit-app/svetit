#include "cc_type_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcTypeParam& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["ccTypeId"] = item.ccTypeId;
	builder["paramId"] = item.paramId;

	return builder.ExtractValue();
}

CcTypeParam Parse(
	const formats::json::Value& json,
	formats::parse::To<CcTypeParam>)
{
	return {
		.ccTypeId = json["ccTypeId"].As<int64_t>(),
		.paramId = json["paramId"].As<int64_t>()
	};
}

} // namespace svetit::project::model