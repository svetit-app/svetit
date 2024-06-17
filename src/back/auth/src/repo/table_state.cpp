#include "table_state.hpp"

#include <memory>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::auth::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

State::State(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{
}

const storages::postgres::Query kInsertState{
	"INSERT INTO auth.state (state, redirect_url) "
	"VALUES ($1, $2) "
	"ON CONFLICT DO NOTHING",
	storages::postgres::Query::Name{"insert_state"},
};

void State::Save(
	const std::string& state,
	const std::string& redirectUrl)
{
	// storages::postgres::Transaction transaction =
	// 	_pg->Begin("insert_state_transaction",
	// 		ClusterHostType::kMaster, {});

	// todo - is it right conversion?
	auto transaction = _db->WithTrx();

	transaction.Execute(kInsertState, state, redirectUrl);
	transaction.Commit();
}

const storages::postgres::Query kSelectState{
	"SELECT id, redirect_url FROM auth.state WHERE state=$1",
	storages::postgres::Query::Name{"select_state"},
};

std::string State::Take(const std::string& state)
{
	// storages::postgres::Transaction transaction =
	// 	_pg->Begin("take_state_transaction",
	// 		ClusterHostType::kMaster, {});

	// is it right conversion?
	auto transaction =
		_db->WithTrx();

	transaction.Execute("DELETE FROM auth.state WHERE created_at <= EXTRACT(EPOCH FROM CURRENT_TIMESTAMP)::BIGINT - 86400");
	auto res = transaction.Execute(kSelectState, state);
	if (res.IsEmpty())
	{
		transaction.Commit();
		return {};
	}

	auto id = res.Front()[0].As<int64_t>();
	auto redirectUrl = res.Front()[1].As<std::string>();

	transaction.Execute("DELETE FROM auth.state WHERE id=$1", id);
	transaction.Commit();

	return redirectUrl;
}

} // namespace svetit::auth::table
