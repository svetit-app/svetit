#pragma once

#include "../model/cc_mode_type.hpp"
#include <svetit/db/db_table.hpp>

namespace svetit::project::table {

class CcModeType final : public db::Table<model::CcModeType> {
public:
	using db::Table<model::CcModeType>::Table;
	PagingResult<model::CcModeType> GetListByProjectId(
		int start, int limit,
		const boost::uuids::uuid& spaceId,
		const boost::uuids::uuid& projectId);
};

} // namespace svetit::project::table
