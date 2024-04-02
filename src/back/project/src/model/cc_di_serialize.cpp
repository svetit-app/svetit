#include "cc_di_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcDi& ccDi,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["ccId"] = ccDi.ccId;
	builder["diId"] = ccDi.diId;
	builder["isDeleted"] = ccDi.isDeleted;

	return builder.ExtractValue();
}

CcDi Parse(
	const formats::json::Value& json,
	formats::parse::To<CcDi>)
{
	return {
		.ccId = json["ccId"].As<int>(),
		.diId = json["diId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model