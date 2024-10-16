#include "table_project.hpp"
#include <svetit/errors.hpp>

#include <userver/utils/boost_uuid4.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

const pg::Query kSelectByKey{
	"SELECT id, space_id, key, name, description, changed_at, sync FROM project.project WHERE key = $1 AND space_id = $2",
	pg::Query::Name{"select_project_by_key"},
};

model::Project Project::GetByKey(const boost::uuids::uuid& spaceId, const std::string& key) {
	auto res = _db->Execute(ClusterHostType::kMaster, kSelectByKey, key, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Project>(pg::kRowTag);
}

} // namespace svetit::project::table
