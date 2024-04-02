#include "cc_status_type_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcStatusType& ccStatusType,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = ccStatusType.id;
	builder["ccTypeId"] = ccStatusType.ccTypeId;
	builder["categoryId"] = ccStatusType.categoryId;
	builder["key"] = ccStatusType.key;
	builder["text"] = ccStatusType.text;
	builder["inform"] = ccStatusType.inform;
	builder["isDeleted"] = ccStatusType.isDeleted;

	return builder.ExtractValue();
}

CcStatusType Parse(
	const formats::json::Value& json,
	formats::parse::To<CcStatusType>)
{
	return {
		.id = json["id"].As<int>(),
		.ccTypeId = json["ccTypeId"].As<int>(),
		.categoryId = json["categoryId"].As<int>(),
		.key = json["key"].As<std::string>(),
		.text = json["text"].As<std::string>(),
		.inform = json["inform"].As<bool>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model