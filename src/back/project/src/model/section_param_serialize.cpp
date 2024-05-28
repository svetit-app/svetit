#include "section_param_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const SectionParam& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["sectionId"] = item.sectionId;
	builder["paramId"] = item.paramId;

	return builder.ExtractValue();
}

SectionParam Parse(
	const formats::json::Value& json,
	formats::parse::To<SectionParam>)
{
	return {
		.spaceId = {},
		.sectionId = json["sectionId"].As<int64_t>(),
		.paramId = json["paramId"].As<int64_t>()
	};
}

} // namespace svetit::project::model