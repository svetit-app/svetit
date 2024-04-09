#include "save_timer_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const SaveTimer& saveTimer,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = saveTimer.id;
	builder["projectId"] = boost::uuids::to_string(saveTimer.projectId);
	builder["intervalMsec"] = saveTimer.intervalMsec;

	return builder.ExtractValue();
}

SaveTimer Parse(
	const formats::json::Value& json,
	formats::parse::To<SaveTimer>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.id = json["id"].As<int>(),
		.projectId = projectId,
		.intervalMsec = json["intervalMsec"].As<int>()
	};
}

} // namespace svetit::project::model