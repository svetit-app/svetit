#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utils/trivial_map.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

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

formats::json::Value Serialize(
	const SaveAlgorithm::Type& item,
	formats::serialize::To<formats::json::Value>
);

SaveAlgorithm::Type Parse(
	const formats::json::Value& json,
	formats::parse::To<SaveAlgorithm::Type>
);

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