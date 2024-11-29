#include "repository.hpp"

#include <svetit/errors.hpp>
#include <svetit/db/db_base.hpp>
#include <svetit/time_utils.hpp>

#include <userver/storages/postgres/postgres_fwd.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::dtlsgateway {

namespace pg = storages::postgres;
using pg::ClusterHostType;

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
{}

Repository Repository::WithTrx(const pg::TransactionOptions& opt) {
	return Repository{std::make_shared<db::Base>(_db->WithTrx(opt))};
}

void Repository::Commit() {
	_db->Commit();
}

} // namespace svetit::dtlsgateway
