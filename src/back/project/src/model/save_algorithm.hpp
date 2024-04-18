#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svetit::project {

struct SaveAlgorithm {
	enum Type : int {
		Off,
		Immediately,
		ByTimer,
		ByTimerOrImmediately,
	};

	static std::string ToString(const Type& saveAlgorithm);
	static Type FromString(const std::string& saveAlgorithm);
};

} // namespace svetit::project

template <>
struct storages::postgres::io::CppToUserPg<svetit::project::SaveAlgorithm::Type> {
	static constexpr DBTypeName postgres_name = "project.save_algorithm";
	static constexpr userver::utils::TrivialBiMap enumerators =
		[](auto selector) {
		return selector()
			.Case("off", svetit::project::SaveAlgorithm::Type::Off)
			.Case("immediately", svetit::project::SaveAlgorithm::Type::Immediately)
			.Case("byTimer", svetit::project::SaveAlgorithm::Type::ByTimer)
			.Case("byTimerOrImmediately", svetit::project::SaveAlgorithm::Type::ByTimerOrImmediately);
	};
};