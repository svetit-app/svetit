#include "cc_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcParam& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["ccId"] = item.ccId;
	builder["paramId"] = item.paramId;

	return builder.ExtractValue();
}

CcParam Parse(
	const formats::json::Value& json,
	formats::parse::To<CcParam>)
{
	return {
		.ccId = json["ccId"].As<int64_t>(),
		.paramId = json["paramId"].As<int64_t>()
	};
}

} // namespace svetit::project::model