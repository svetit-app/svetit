#pragma once

#include "../model/model.hpp"

#include "table_state.hpp"
#include "table_session.hpp"

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::auth {

class Repository {
public:
	Repository(storages::postgres::ClusterPtr pg);
	Repository(std::shared_ptr<db::Base> dbPtr);

	table::State& State();
	table::Session& Session();

	Repository WithTrx(const storages::postgres::TransactionOptions& opt = storages::postgres::Transaction::RW);
	void Commit();
	void Rollback();

private:
	std::shared_ptr<db::Base> _db;
	table::State _state;
	table::Session _session;
};

class RepositoryComponent final : public components::LoggableComponentBase, public Repository {
public:
	static constexpr std::string_view kName = "repository";
	static yaml_config::Schema GetStaticConfigSchema();
	explicit RepositoryComponent(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);
};

} // namespace svetit::auth
