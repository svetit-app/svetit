#include "table_session.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::auth::table {

Session::Session(
	    storages::postgres::ClusterPtr pg
        )
	: _pg{pg}
{
constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS session (
 	id SERIAL,
 	token TEXT NOT NULL,
 	created TIMESTAMPTZ NOT NULL DEFAULT NOW(),
 	expired TIMESTAMPTZ NOT NULL,
 	userId VARCHAR(255) NOT NULL,
 	device VARCHAR(25) NOT NULL,
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

// void svetit::auth::table::Session::Save(
//         const std::string& token,
//         const std::chrono::system_clock::time_point& created,
//         const std::chrono::system_clock::time_point& expired,
//         const std::string& userId,
//         const std::string& device,
//         const std::string& accessToken,
//         const std::string& refreshToken,
//         const std::string& idToken,
//         const bool& active)
// {

// 	storages::postgres::Transaction transaction =
// 		_pg->Begin("insert_session_transaction",
// 			storages::postgres::ClusterHostType::kMaster, {});

// 	transaction.Execute(kInsertSession, token, created, expired, userId, device, accessToken, refreshToken, idToken, active);
// 	transaction.Commit();
// }

// const storages::postgres::Query kSelectState{
// 	"SELECT id, redirectUrl FROM states WHERE state=$1",
// 	storages::postgres::Query::Name{"select_state"},
// };

// std::string State::Take(const std::string& state)
// {
// 	storages::postgres::Transaction transaction =
// 		_pg->Begin("take_state_transaction",
// 			storages::postgres::ClusterHostType::kMaster, {});

// 	transaction.Execute("DELETE FROM states WHERE created <= NOW() - interval '1' day");
// 	auto res = transaction.Execute(kSelectState, state);
// 	if (res.IsEmpty())
// 	{
// 		transaction.Commit();
// 		return {};
// 	}

// 	auto id = res.Front()[0].As<int64_t>();
// 	auto redirectUrl = res.Front()[1].As<std::string>();

// 	transaction.Execute("DELETE FROM states WHERE id=$1", id);
// 	transaction.Commit();

// 	return redirectUrl;
// }

} // namespace svetit::auth::table
