#include "table_role.hpp"
#include <memory>
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Role::Role(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kInsertSystemRoles{
	R"~(
		INSERT INTO space.role (id, space_id, name) VALUES
			(1, NULL, 'operator'),
			(2, NULL, 'user'),
			(3, NULL, 'admin')
		ON CONFLICT DO NOTHING;
	)~",
	pg::Query::Name{"role.insert_system_roles"},
};

const pg::Query kRoleIdSeq{
	R"~(
		SELECT setval('space.role_id_seq', max(id)) FROM space.role;
	)~",
	pg::Query::Name{"role.set_role_id_seq"},
};

void Role::CreateSystemRoles() {
	_db->Execute(ClusterHostType::kMaster, kInsertSystemRoles);
	_db->Execute(ClusterHostType::kMaster, kRoleIdSeq);
}

const pg::Query kSelect{
	"SELECT id, space_id, name FROM space.role WHERE id = $1 AND (space_id IS NULL OR space_id = $2)",
	pg::Query::Name{"role.select"},
};

model::Role Role::Select(int id, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelect, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Role>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO space.role (space_id, name) "
	"VALUES ($1, $2) RETURNING id",
	pg::Query::Name{"role.insert"},
};

int Role::Create(const std::string& roleName, const boost::uuids::uuid& spaceId) {
	const auto res = _db->Execute(ClusterHostType::kMaster, kInsert, spaceId, roleName);
	return res.AsSingleRow<int>();
}

const pg::Query kDelete {
	"DELETE FROM space.role WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"role.delete"},
};

void Role::Delete(int id, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kUpdate {
	"UPDATE space.role SET name = $3 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"role.update"},
};

void Role::Update(const model::Role& item, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kUpdate, item.id, spaceId, item.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectListBySpaceId {
	"SELECT id, space_id, name, COUNT(*) OVER() FROM space.role WHERE (space_id = $1 OR space_id IS NULL) OFFSET $2 LIMIT $3",
	pg::Query::Name{"role.select_role_list_by_spaceId"},
};

PagingResult<model::Role> Role::SelectList(int32_t start, int32_t limit, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectListBySpaceId, spaceId, start, limit);
	PagingResult<model::Role> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::space::table
