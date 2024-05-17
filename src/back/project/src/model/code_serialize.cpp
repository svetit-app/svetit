#include "code_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Code& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["projectId"] = boost::uuids::to_string(item.projectId);
	builder["repositoryId"] = boost::uuids::to_string(item.repositoryId);
	builder["commitHash"] = item.commitHash;

	return builder.ExtractValue();
}

Code Parse(
	const formats::json::Value& json,
	formats::parse::To<Code>)
{
	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	const auto repositoryIdStr = json["repositoryId"].As<std::string>();
	const auto repositoryId = repositoryIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(repositoryIdStr);

	return {
		.id = json["id"].As<int64_t>(),
		.projectId = projectId,
		.repositoryId = repositoryId,
		.commitHash = json["commitHash"].As<std::string>(),
	};
}

} // namespace svetit::project::model