#include "sync_direction.hpp"
#include <shared/errors.hpp>

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

} // namespace svetit::project
