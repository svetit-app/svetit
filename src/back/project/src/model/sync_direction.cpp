#include "sync_direction.hpp"
#include <svetit/errors.hpp>

namespace svetit::project {

/*static*/ std::string SyncDirection::ToString(const SyncDirection::Type& syncDirection)
{
	switch (syncDirection) {
	case ProjectToNode:
		return "projectToNode";
	case NodeToProject:
		return "nodeToProject";
	default:
		break;
	}

	return {};
}

/*static*/ SyncDirection::Type SyncDirection::FromString(const std::string& syncDirection)
{
	if (syncDirection == "projectToNode")
		return ProjectToNode;
	if (syncDirection == "nodeToProject")
		return NodeToProject;

	throw errors::BadRequest400("Wrong sync direction");
}

formats::json::Value Serialize(
	const SyncDirection::Type& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder res{SyncDirection::ToString(item)};
	return res.ExtractValue();
}

SyncDirection::Type Parse(
	const formats::json::Value& json,
	formats::parse::To<SyncDirection::Type>)
{
	return SyncDirection::FromString(json.As<std::string>());
}

} // namespace svetit::project
