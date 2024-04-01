#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project {

struct DiMode {
	enum Type : int {
		ReadonlyFlag,
		ReadwriteFlag,
		Readonly,
		Readwrite,
		File,
		Button,
		VideoStream,
	};
};

} // namespace svetit::project

template <>
struct storages::postgres::io::traits::CanUseEnumAsStrongTypedef<svetit::project::DiMode::Type> : std::true_type {};