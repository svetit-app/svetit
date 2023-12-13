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
using pg::ClusterHostType;

Session::Session(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
}

const storages::postgres::Query kQuerySave{
	"INSERT INTO auth.session (id, createdAt, expiredAt, token, userId, device, accessToken, refreshToken, idToken, active)"
	"VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10) "
	"ON CONFLICT DO NOTHING",
	storages::postgres::Query::Name{"session_save"},
};

void Session::Save(const model::Session& data)
{
	storages::postgres::Transaction t =
		_pg->Begin("insert_session_transaction",
			ClusterHostType::kMaster, {});

	std::apply([&t](const auto&... args) {
		t.Execute(kQuerySave, args...);
	}, boost::pfr::structure_tie(data));

	t.Commit();
}

const storages::postgres::Query kQueryGetWithActive{
	"SELECT id, createdAt, expiredAt, token, userId, device, "
	"accessToken, refreshToken, idToken, active FROM auth.session "
	"WHERE id=$1 AND active=$2",
	storages::postgres::Query::Name{"session_get_with_active"},
};

const storages::postgres::Query kQueryGet{
	"SELECT id, createdAt, expiredAt, token, userId, device, "
	"accessToken, refreshToken, idToken, active FROM session "
	"WHERE id=$1",
	storages::postgres::Query::Name{"session_get"},
};

model::Session Session::Get(const std::string& id, const std::optional<bool>& isActive)
{
	auto res = isActive.has_value() ?
		_pg->Execute(ClusterHostType::kSlave, kQueryGetWithActive,
			utils::BoostUuidFromString(id), isActive.value()) :
		_pg->Execute(ClusterHostType::kSlave, kQueryGet,
			utils::BoostUuidFromString(id));
	if (res.IsEmpty())
		throw errors::NotFound404{};

	auto items = res.AsContainer<std::vector<model::Session>>(pg::kRowTag);
	if (items.empty())
		throw errors::NotFound404{};

	return items.front();
}

const storages::postgres::Query kQueryUpdateTokens{
	"UPDATE auth.session SET accessToken = $2, refreshToken = $3, idToken = $4 "
	"WHERE id=$1",
	storages::postgres::Query::Name{"session_update_tokens"},
};

void Session::UpdateTokens(const model::Session& s)
{
	_pg->Execute(ClusterHostType::kMaster, kQueryUpdateTokens,
		s._id, s._accessToken, s._refreshToken, s._idToken);
}

const storages::postgres::Query kQueryInactivateByUserId{
	"UPDATE auth.session SET active=false, token='', accessToken='', refreshToken='', idToken='' "
	"WHERE active=true AND userId=$1",
	storages::postgres::Query::Name{"session_inactivate_by_userId"},
};

const storages::postgres::Query kQueryInactivateById{
	"UPDATE auth.session SET active=false, token='', accessToken='', refreshToken='', idToken='' "
	"WHERE active=true AND id=$1",
	storages::postgres::Query::Name{"session_inactivate"},
};

void Session::MarkInactive(const boost::uuids::uuid& id)
{
	_pg->Execute(ClusterHostType::kMaster, kQueryInactivateById, id);
}

void Session::MarkInactive(const std::string& userId)
{
	_pg->Execute(ClusterHostType::kMaster, kQueryInactivateByUserId, userId);
}

bool Session::Refresh(
	const model::Session& data,
	const boost::uuids::uuid& oldId)
{
	storages::postgres::Transaction t =
		_pg->Begin("refresh_session_transaction",
			ClusterHostType::kMaster, {});

	std::apply([&t](const auto&... args) {
		t.Execute(kQuerySave, args...);
	}, boost::pfr::structure_tie(data));

	auto res = t.Execute(kQueryInactivateById, oldId);
	if (res.RowsAffected() < 1) {
		t.Rollback();
		return false;
	}

	t.Commit();
	return true;
}

} // namespace svetit::auth::table
