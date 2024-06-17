#include "repository.hpp"

#include "table_state.hpp"
#include "table_session.hpp"

#include "shared/db/db_base.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>
#include "userver/storages/postgres/postgres_fwd.hpp"
#include <memory>

namespace svetit::auth {

/*static*/ yaml_config::Schema RepositoryComponent::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main repository component
additionalProperties: false
properties:
  some-var:
    type: string
    description: some desc
)");
}

RepositoryComponent::RepositoryComponent(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, Repository{ctx.FindComponent<components::Postgres>("database").GetCluster()}
{}

Repository::Repository(storages::postgres::ClusterPtr pg)
	: Repository{std::make_shared<db::Base>(std::move(pg))} {}

Repository::Repository(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
	, _state{_db}
	, _session{_db}
{}

table::State& Repository::State(){
	return _state;
}

table::Session& Repository::Session(){
	return _session;
}

Repository Repository::WithTrx(const userver::storages::postgres::TransactionOptions& opt) {
	return Repository{std::make_shared<db::Base>(_db->WithTrx(opt))};
}

void Repository::Commit() {
	_db->Commit();
}

void Repository::Rollback() {
	// todo - need to write/extend it?
	//_db->Rollback;
}

} // namespace svetit::auth