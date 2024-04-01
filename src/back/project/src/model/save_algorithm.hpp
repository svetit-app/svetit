#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project {

struct SaveAlgorithm {
	enum Type : int {
		Off,
		Immediately,
		ByTimer,
		ByTimerOrImmediately,
	};
};

} // namespace svetit::project

template <>
struct storages::postgres::io::traits::CanUseEnumAsStrongTypedef<svetit::project::SaveAlgorithm::Type> : std::true_type {};