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
	explicit Session(storages::postgres::ClusterPtr pg);

	void Save(const model::Session& data);

	model::Session GetById(const std::string& id, bool isActive = true);

	void UpdateTokens(const model::Session& s);

	void MarkInactive(const model::Session& s);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::auth::table
