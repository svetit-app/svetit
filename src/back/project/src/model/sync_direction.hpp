#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project {

struct SyncDirection {
	enum Type : int {
		ProjectToNode,
		NodeToProject,
	};
};

} // namespace svetit::project

template <>
struct storages::postgres::io::traits::CanUseEnumAsStrongTypedef<svetit::project::SyncDirection::Type> : std::true_type {};