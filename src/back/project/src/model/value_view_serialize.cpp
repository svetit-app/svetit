#include "value_view_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ValueView& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["diTypeId"] = item.diTypeId;
	builder["value"] = item.value;
	builder["view"] = item.view;
	if (item.isDeleted)
		builder["isDeleted"] = item.isDeleted;

	return builder.ExtractValue();
}

ValueView Parse(
	const formats::json::Value& json,
	formats::parse::To<ValueView>)
{
	return {
		.id = json["id"].As<int>(),
		.diTypeId = json["diTypeId"].As<int>(),
		.value = json["value"].As<std::string>(),
		.view = json["view"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>()
	};
}

} // namespace svetit::project::model