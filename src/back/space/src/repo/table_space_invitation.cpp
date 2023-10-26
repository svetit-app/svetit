#include "table_space_invitation.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceInvitation::SpaceInvitation(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_invitation (
	id serial,
	spaceId uuid,
	creatorId uuid,
	userId uuid,
	role TEXT NOT NULL,
	createdAt TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);
}

} // namespace svetit::space::table
