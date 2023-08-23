#pragma once

#include "../model/model.hpp"

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::auth::table {

class State final {
public:
	static constexpr std::string_view kName = "state";

	explicit State(
	    storages::postgres::ClusterPtr pg);

	void Save(
		const std::string& state,
		const std::string& redirectUrl);

	std::string Take(const std::string& state);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::auth