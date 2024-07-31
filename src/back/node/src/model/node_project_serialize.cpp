#include "node_project_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const NodeProject& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["nodeId"] = boost::uuids::to_string(item.nodeId);
	builder["projectId"] = boost::uuids::to_string(item.projectId);

	return builder.ExtractValue();
}

NodeProject Parse(
	const formats::json::Value& json,
	formats::parse::To<NodeProject>)
{
	const auto nodeIdStr = json["nodeId"].As<std::string>();
	const auto nodeId = nodeIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(nodeIdStr);

	const auto projectIdStr = json["projectId"].As<std::string>();
	const auto projectId = projectIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(projectIdStr);

	return {
		.nodeId = nodeId,
		.projectId = projectId
	};
}

} // namespace svetit::node::model