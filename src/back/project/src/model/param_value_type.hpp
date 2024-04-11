#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>

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
struct storages::postgres::io::traits::CanUseEnumAsStrongTypedef<svetit::project::ParamValueType::Type> : std::true_type {};