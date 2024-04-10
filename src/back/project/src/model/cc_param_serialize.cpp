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
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

	return builder.ExtractValue();
}

CcParam Parse(
	const formats::json::Value& json,
	formats::parse::To<CcParam>)
{
	return {
		.ccId = json["ccId"].As<int>(),
		.paramId = json["paramId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model