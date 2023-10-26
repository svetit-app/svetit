#include "table_space_user.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceUser::SpaceUser(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_user (
	spaceId uuid,
	userId uuid,
	isOwner BOOLEAN NOT NULL,
	joinedAt TIMESTAMPTZ NOT NULL DEFAULT NOW(),
	role TEXT NOT NULL,
	requestsAllowed BOOLEAN NOT NULL
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);
}

} // namespace svetit::space::table
