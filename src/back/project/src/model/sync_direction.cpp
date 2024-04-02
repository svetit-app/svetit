#include "sync_direction.hpp"
#include <shared/errors.hpp>

namespace svetit::project {

/*static*/ std::string SyncDirection::ToString(const SyncDirection::Type& syncDirection)
{
	switch (syncDirection) {
	case ProjectToNode:
		return "project_to_node";
	case NodeToProject:
		return "node_to_project";
	default:
		break;
	}

	return {};
}

/*static*/ SyncDirection::Type SyncDirection::FromString(const std::string& syncDirection)
{
	if (syncDirection == "project_to_node")
		return ProjectToNode;
	if (syncDirection == "node_to_project")
		return NodeToProject;

	throw errors::BadRequest400("Wrong sync direction");
}

} // namespace svetit::project
