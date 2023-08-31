#pragma once

#include "../model/session.hpp"

#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::auth::table {

class Session final {
public:
	static constexpr std::string_view kName = "session";

	explicit Session(storages::postgres::ClusterPtr pg);

	void Save(const model::Session& data);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::auth::table
