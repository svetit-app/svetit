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
	builder["createdAt"] = std::chrono::duration_cast<std::chrono::seconds>(s.createdAt.time_since_epoch()).count();

	return builder.ExtractValue();
}

Space Parse(
	const formats::json::Value& json,
	formats::parse::To<Space>)
{
	const auto idStr = json["id"].As<std::string>("");
	const auto id = idStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(idStr);

	const std::chrono::system_clock::time_point createdAt{std::chrono::seconds{json["createdAt"].As<int64_t>(0)}};

	return Space{
		.id = id,
		.name = json["name"].As<std::string>(),
		.key = json["key"].As<std::string>(),
		.requestsAllowed = json["requestsAllowed"].As<bool>(),
		.createdAt = createdAt
	};
}

} // namespace svetit::space::model
