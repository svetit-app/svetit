#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svetit::project {

struct SyncDirection {
	enum Type : int {
		ProjectToNode,
		NodeToProject,
	};

	static std::string ToString(const Type& syncDirection);
	static Type FromString(const std::string& syncDirection);
};

} // namespace svetit::project

template <>
struct storages::postgres::io::CppToUserPg<svetit::project::SyncDirection::Type> {
	static constexpr DBTypeName postgres_name = "project.sync_direction";
	static constexpr userver::utils::TrivialBiMap enumerators =
		[](auto selector) {
		return selector()
			.Case("projectToNode", svetit::project::SyncDirection::Type::ProjectToNode)
			.Case("nodeToProject", svetit::project::SyncDirection::Type::NodeToProject);
	};
};
