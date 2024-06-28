#include "table_cc_mode_type.hpp"

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

const pg::Query kGetListByProject{
	"SELECT id, space_id, project_id, cc_type_id, key, name, COUNT(*) OVER() FROM project.cc_mode_type "
	"WHERE space_id = $1 AND project_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_mode_types_by_project"},
};

PagingResult<model::CcModeType> CcModeType::GetListByProjectId(
		int start, int limit,
		const boost::uuids::uuid& spaceId,
		const boost::uuids::uuid& projectId)
{
	auto res = _db->Execute(ClusterHostType::kSlave, kGetListByProject, spaceId, projectId, start, limit);

	PagingResult<model::CcModeType> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::project::table
