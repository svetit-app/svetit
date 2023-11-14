#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space {

struct Role {
	enum Type : int {
		Unknown,
		Guest,
		User,
		Admin
	};

	static std::string ToString(const Type& role);
	static Type FromString(const std::string& role);
};

} // namespace svetit::space

template <>
struct storages::postgres::io::traits::CanUseEnumAsStrongTypedef<svetit::space::Role::Type> : std::true_type {};