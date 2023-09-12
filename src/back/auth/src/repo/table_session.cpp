#include "table_session.hpp"
#include "../../../shared/errors.hpp"
#include "../model/errors.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::auth::table {

namespace pg = storages::postgres;

Session::Session(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS session (
	id uuid,
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
	"INSERT INTO session (id, created, expired, token, userId, device, accessToken, refreshToken, idToken, active)"
	"VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10) "
	"ON CONFLICT DO NOTHING",
	storages::postgres::Query::Name{"insert_session"},
};

void Session::Save(const model::Session& data)
{
	storages::postgres::Transaction t =
		_pg->Begin("insert_session_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	std::apply([&t](const auto&... args) {
		t.Execute(kInsertSession, args...);
	}, boost::pfr::structure_tie(data));

	t.Commit();
}

const storages::postgres::Query kSelectSession{
	"SELECT id, created, expired, token, userId, device, "
	"accessToken, refreshToken, idToken, active FROM session "
	"WHERE id=$1 AND active=$2",
	storages::postgres::Query::Name{"select_session"},
};

model::Session Session::GetById(const std::string& id, bool isActive)
{
	storages::postgres::ResultSet res = _pg->Execute(
		storages::postgres::ClusterHostType::kSlave, kSelectSession,
		utils::BoostUuidFromString(id), isActive);
	if (res.IsEmpty())
		throw errors::NotFound{};

	auto items = res.AsContainer<std::vector<model::Session>>(pg::kRowTag);
	if (items.empty())
		throw errors::NotFound{};

	return items.front();
}

const storages::postgres::Query kGetSession{
	"SELECT id, created, expired, token, userId, device, "
	"accessToken, refreshToken, idToken, active FROM session "
	"WHERE id=$1",
	storages::postgres::Query::Name{"get_session"},
};

model::Session Session::Get(const std::string& id)
{
	storages::postgres::ResultSet res = _pg->Execute(
		storages::postgres::ClusterHostType::kSlave, kGetSession,
		utils::BoostUuidFromString(id));
	if (res.IsEmpty())
		throw errors::NotFound{};

	auto items = res.AsContainer<std::vector<model::Session>>(pg::kRowTag);
	if (items.empty())
		throw errors::NotFound{};

	return items.front();
}



const storages::postgres::Query kInactivateSessionsWithUserId{
	"UPDATE session SET active=false, token='', accessToken='', refreshToken='', idToken='' "
	"WHERE userId=$1",
	storages::postgres::Query::Name{"inactivate-sessions"},
};

const storages::postgres::Query kUpdateTokens{
	"UPDATE session SET accessToken = $2, refreshToken = $3, idToken = $4 "
	"WHERE id=$1",
	storages::postgres::Query::Name{"update_tokens"},
};

void Session::UpdateTokens(const model::Session& s)
{
	_pg->Execute(
		storages::postgres::ClusterHostType::kMaster, kUpdateTokens,
		s._id, s._accessToken, s._refreshToken, s._idToken);
}

const storages::postgres::Query kMarkSessionInactive{
	"UPDATE session SET active = false, token='', accessToken='', refreshToken='', idToken='' "
	"WHERE id=$1 AND active=true",
	storages::postgres::Query::Name{"mark_session_inactive"},
};


void Session::BlockEverySessionByUser(const std::string& userId)
{
	_pg->Execute(
		storages::postgres::ClusterHostType::kMaster, kInactivateSessionsWithUserId,
		userId);
}

bool Session::Refresh(
	const model::Session& data,
	const boost::uuids::uuid& oldId)
{
	storages::postgres::Transaction t =
		_pg->Begin("refresh_session_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	std::apply([&t](const auto&... args) {
		t.Execute(kInsertSession, args...);
	}, boost::pfr::structure_tie(data));

	auto res = t.Execute(kMarkSessionInactive, oldId);
	if (res.RowsAffected() < 1) {
		t.Rollback();
		return false;
	}

	t.Commit();
	return true;
}

void Session::MarkInactive(const boost::uuids::uuid& sid)
{
	_pg->Execute(
		storages::postgres::ClusterHostType::kMaster, kMarkSessionInactive, sid);
}

} // namespace svetit::auth::table
