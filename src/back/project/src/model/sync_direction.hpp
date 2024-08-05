#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utils/trivial_map.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project {

struct SyncDirection {
	enum Type : int {
		ProjectToNode,
		NodeToProject,
	};

	static std::string ToString(const Type& syncDirection);
	static Type FromString(const std::string& syncDirection);
};

formats::json::Value Serialize(
	const SyncDirection::Type& item,
	formats::serialize::To<formats::json::Value>);

SyncDirection::Type Parse(const formats::json::Value& json,
	formats::parse::To<SyncDirection::Type>);

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
