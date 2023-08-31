#include "table_session.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::auth::table {

Session::Session(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS session (
 	id SERIAL,
 	token TEXT NOT NULL,
 	created TIMESTAMPTZ NOT NULL DEFAULT NOW(),
 	expired TIMESTAMPTZ NOT NULL,
 	userId VARCHAR(36) NOT NULL,
 	device TEXT NOT NULL,
 	accessToken TEXT NOT NULL,
 	refreshToken TEXT NOT NULL,
 	idToken TEXT NOT NULL,
 	active BOOLEAN NOT NULL
);
)~";

 	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);
}

const storages::postgres::Query kInsertSession{
	"INSERT INTO session (token, created, expired, userId, device, accessToken, refreshToken, idToken, active)"
	"VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9) "
	"ON CONFLICT DO NOTHING",
	storages::postgres::Query::Name{"insert_session"},
};

void Session::Save(SessionData& data)
{

	storages::postgres::Transaction transaction =
		_pg->Begin("insert_session_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertSession, data._token, data._created, data._expired, data._userId, data._device, data._accessToken, data._refreshToken, data._idToken, data._active);
	transaction.Commit();
}

} // namespace svetit::auth::table
