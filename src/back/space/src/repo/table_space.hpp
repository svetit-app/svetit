#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class Space final {
public:
	explicit Space(storages::postgres::ClusterPtr pg);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
