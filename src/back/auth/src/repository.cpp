#include "repository.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::auth {

/*static*/ yaml_config::Schema Repository::GetStaticConfigSchema()
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

Repository::Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _pg{ctx.FindComponent<components::Postgres>("database").GetCluster()}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS states (
	id SERIAL,
	state TEXT NOT NULL,
	redirectUrl TEXT NOT NULL,
	created TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);
}

const storages::postgres::Query kInsertState{
	"INSERT INTO states (state, redirectUrl) "
	"VALUES ($1, $2) "
	"ON CONFLICT DO NOTHING",
	storages::postgres::Query::Name{"insert_state"},
};

void Repository::SaveState(
	const std::string& state,
	const std::string& redirectUrl)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("insert_state_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertState, state, redirectUrl);
	transaction.Commit();
}

const storages::postgres::Query kSelectState{
	"SELECT id, redirectUrl FROM states WHERE state=$1",
	storages::postgres::Query::Name{"select_state"},
};

std::string Repository::TakeState(const std::string& state)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("take_state_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute("DELETE FROM states WHERE created <= NOW() - interval '1' day");
	auto res = transaction.Execute(kSelectState, state);
	if (res.IsEmpty())
	{
		transaction.Commit();
		return {};
	}

	auto id = res.Front()[0].As<int64_t>();
	auto redirectUrl = res.Front()[1].As<std::string>();

	transaction.Execute("DELETE FROM states WHERE id=$1", id);
	transaction.Commit();

	return redirectUrl;
}

} // namespace svetit::auth
