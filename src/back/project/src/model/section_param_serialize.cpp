#include "section_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const SectionParam& sectionParam,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["sectionId"] = sectionParam.sectionId;
	builder["paramId"] = sectionParam.paramId;
	builder["isDeleted"] = sectionParam.isDeleted;

	return builder.ExtractValue();
}

SectionParam Parse(
	const formats::json::Value& json,
	formats::parse::To<SectionParam>)
{
	return {
		.sectionId = json["sectionId"].As<int>(),
		.paramId = json["paramId"].As<int>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model