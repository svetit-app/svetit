#pragma once

#include "table_space.hpp"
#include "table_space_user.hpp"
#include "table_space_invitation.hpp"
#include "table_space_link.hpp"

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space {

class Repository final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "repository";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	table::Space& Space();
	table::SpaceUser& SpaceUser();
	table::SpaceInvitation& SpaceInvitation();
	table::SpaceLink& SpaceLink();

private:
	storages::postgres::ClusterPtr _pg;
	table::Space _space;
	table::SpaceUser _spaceUser;
	table::SpaceInvitation _spaceInvitation;
	table::SpaceLink _spaceLink;
};

} // namespace svetit::space