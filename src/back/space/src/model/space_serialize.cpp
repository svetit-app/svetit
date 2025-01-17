#include "space_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const Space& s,
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
	const auto idStr = json["id"].As<std::string>("");
	const auto id = idStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(idStr);

	return Space{
		.id = id,
		.name = json["name"].As<std::string>(),
		.key = json["key"].As<std::string>(),
		.requestsAllowed = json["requestsAllowed"].As<bool>(),
		.createdAt = json["createdAt"].As<int64_t>(0)
	};
}

} // namespace svetit::space::model
