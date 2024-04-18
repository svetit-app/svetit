#pragma once

#include <userver/storages/postgres/io/strong_typedef.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/trivial_map.hpp>

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

	static std::string ToString(const Type& diMode);
	static Type FromString(const std::string& diMode);
};

} // namespace svetit::project

template <>
struct storages::postgres::io::CppToUserPg<svetit::project::DiMode::Type> {
	static constexpr DBTypeName postgres_name = "project.di_mode";
	static constexpr userver::utils::TrivialBiMap enumerators =
		[](auto selector) {
		return selector()
			.Case("readonlyFlag", svetit::project::DiMode::Type::ReadonlyFlag)
			.Case("readwriteFlag", svetit::project::DiMode::Type::ReadwriteFlag)
			.Case("readonly", svetit::project::DiMode::Type::Readonly)
			.Case("readwrite", svetit::project::DiMode::Type::Readwrite)
			.Case("file", svetit::project::DiMode::Type::File)
			.Case("button", svetit::project::DiMode::Type::Button)
			.Case("videoStream", svetit::project::DiMode::Type::VideoStream);
	};
};