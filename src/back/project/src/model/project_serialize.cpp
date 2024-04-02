#include "project_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Project& project,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = boost::uuids::to_string(project.id);
	builder["spaceId"] = boost::uuids::to_string(project.spaceId);
	builder["key"] = project.key;
	builder["name"] = project.name;
	builder["description"] = project.description;
	builder["changedAt"] = std::chrono::duration_cast<std::chrono::seconds>(project.changedAt.time_since_epoch()).count();
	builder["sync"] = SyncDirection::ToString(project.sync);

	return builder.ExtractValue();
}

Project Parse(
	const formats::json::Value& json,
	formats::parse::To<Project>)
{
	const auto idStr = json["id"].As<std::string>();
	const auto id = idStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(idStr);

	const auto spaceIdStr = json["spaceId"].As<std::string>();
	const auto spaceId = spaceIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(spaceIdStr);

	const std::chrono::system_clock::time_point changedAt{std::chrono::seconds{json["changedAt"].As<int64_t>()}};

	return {
		.id = id,
		.spaceId = spaceId,
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.description = json["description"].As<std::string>(),
		.changedAt = changedAt,
		.sync = SyncDirection::FromString(json["sync"].As<std::string>())
	};
}

} // namespace svetit::project::model