#include "repository.hpp"

#include "table_space.hpp"
#include "table_space_user.hpp"
#include "table_space_invitation.hpp"
#include "table_space_link.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space {

/*static*/ yaml_config::Schema Repository::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main repository component
additionalProperties: false
properties:
  some-var:
    type: string
    description: some desc
)");
}

Repository::Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _pg{ctx.FindComponent<components::Postgres>("database").GetCluster()}
	, _space{_pg}
	, _spaceUser{_pg}
	, _spaceInvitation{_pg}
	, _spaceLink{_pg}
{}

table::Space& Repository::Space(){
	return _space;
}

table::SpaceUser& Repository::SpaceUser(){
	return _spaceUser;
}

table::SpaceInvitation& Repository::SpaceInvitation(){
	return _spaceInvitation;
}

table::SpaceLink& Repository::SpaceLink(){
	return _spaceLink;
}

} // namespace svetit::space