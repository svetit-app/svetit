#include "node_group_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const NodeGroup& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["nodeId"] = boost::uuids::to_string(item.nodeId);
	builder["groupId"] = item.groupId;

	return builder.ExtractValue();
}

NodeGroup Parse(
	const formats::json::Value& json,
	formats::parse::To<NodeGroup>)
{
	const auto nodeIdStr = json["nodeId"].As<std::string>();
	const auto nodeId = nodeIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(nodeIdStr);

	return {
		.nodeId = nodeId,
		.groupId = json["groupId"].As<int>()
	};
}

} // namespace svetit::node::model