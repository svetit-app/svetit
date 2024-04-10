#include "cc_di_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcDi& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["ccId"] = item.ccId;
	builder["diId"] = item.diId;
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

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