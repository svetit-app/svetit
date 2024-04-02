#include "translation_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Translation& translation,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = translation.id;
	builder["projectId"] = boost::uuids::to_string(translation.projectId);
	builder["lang"] = translation.lang;
	builder["key"] = translation.key;
	builder["value"] = translation.value;

	return builder.ExtractValue();
}

Translation Parse(
	const formats::json::Value& json,
	formats::parse::To<Translation>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int>(),
		.projectId = projectId,
		.lang = json["lang"].As<std::string>(),
		.key = json["key"].As<std::string>(),
		.value = json["value"].As<std::string>()
	};
}

} // namespace svetit::project::model