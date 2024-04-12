#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/trivial_map.hpp>

namespace svetit::project {

struct ParamValueType {
	enum Type : int {
		Int,
		Bool,
		Float,
		String,
		Bytes,
		Time,
		Range,
		Combo,
	};

	static std::string ToString(const Type& paramValueType);
	static Type FromString(const std::string& paramValueType);
};

} // namespace svetit::project

template <>
struct storages::postgres::io::CppToUserPg<svetit::project::ParamValueType::Type> {
	static constexpr DBTypeName postgres_name = "project.param_value_type";
	static constexpr userver::utils::TrivialBiMap enumerators =
		[](auto selector) {
		return selector()
			.Case("int", svetit::project::ParamValueType::Type::Int)
			.Case("bool", svetit::project::ParamValueType::Type::Bool)
			.Case("float", svetit::project::ParamValueType::Type::Float)
			.Case("string", svetit::project::ParamValueType::Type::String)
			.Case("bytes", svetit::project::ParamValueType::Type::Bytes)
			.Case("time", svetit::project::ParamValueType::Type::Time)
			.Case("range", svetit::project::ParamValueType::Type::Range)
			.Case("combo", svetit::project::ParamValueType::Type::Combo);
	};
};