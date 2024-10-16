#pragma once

#include "../model/model.hpp"
#include <svetit/db/db_base.hpp>

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::auth::table {

class State final {
public:
	explicit State(std::shared_ptr<db::Base> dbPtr);

	void Save(
		const std::string& state,
		const std::string& redirectUrl);

	std::string Take(const std::string& state);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::auth::table
