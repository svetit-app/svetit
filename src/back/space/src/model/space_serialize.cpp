#include "space_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::Space& s,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = boost::uuids::to_string(s.id);
	builder["name"] = s.name;
	builder["key"] = s.key;
	builder["requestAllowed"] = s.requestsAllowed;
	builder["createdAt"] = s.createdAt;

	return builder.ExtractValue();
}

Space Parse(
	const formats::json::Value& json,
	formats::parse::To<Space>)
{
	model::Space space;

	if (json.HasMember("id") && !json["id"].As<std::string>().empty()) {
		space.id = utils::BoostUuidFromString(json["id"].As<std::string>());
	}

	if (json.HasMember("name") && !json["name"].As<std::string>().empty()) {
		space.name = json["name"].As<std::string>();
	}

	if (json.HasMember("key") && !json["key"].As<std::string>().empty()) {
		space.key = json["key"].As<std::string>();
	}

	if (json.HasMember("requestsAllowed")) {
		space.requestsAllowed = json["requestsAllowed"].As<bool>();
	}

	if (json.HasMember("createdAt") && !json["created"].As<std::string>().empty()) {
		space.createdAt = json["created"].As<int64_t>();
	}

	return space;
}

} // namespace svetit::space::model
