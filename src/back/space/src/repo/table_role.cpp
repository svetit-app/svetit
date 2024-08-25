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

const pg::Query kInsertSystemRoles{
	R"~(
		INSERT INTO space.role (id, space_id, name) VALUES
			(1, NULL, 'admin'),
			(2, NULL, 'user'),
			(3, NULL, 'operator')
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
	const auto res = _db->Execute(ClusterHostType::kMaster, kInsertSystemRoles);
	if (res.RowsAffected() > 0)
		_db->Execute(ClusterHostType::kMaster, kRoleIdSeq);
}

const pg::Query kGet{
	"SELECT id, space_id, name FROM space.role WHERE id = $1 AND (space_id IS NULL OR space_id = $2)",
	pg::Query::Name{"role.select"},
};

model::Role Role::Get(int id, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Role>(pg::kRowTag);
}

const pg::Query kGetList {
	"SELECT id, space_id, name, COUNT(*) OVER() FROM space.role WHERE (space_id = $1 OR space_id IS NULL) OFFSET $2 LIMIT $3",
	pg::Query::Name{"role.select_role_list_by_spaceId"},
};

PagingResult<model::Role> Role::GetList(int32_t start, int32_t limit, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kGetList, spaceId, start, limit);
	PagingResult<model::Role> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::space::table
