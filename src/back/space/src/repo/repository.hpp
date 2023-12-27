#pragma once

#include "table_space.hpp"
#include "table_space_user.hpp"
#include "table_space_invitation.hpp"
#include "table_space_link.hpp"

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
	PagingResult<model::Space> SelectAvailable(const std::string& userId, int offset, int limit);
	PagingResult<model::Space> SelectAvailableBySpaceName(const std::string& spaceName, const std::string& userId, int offset, int limit);
	PagingResult<model::Space> SelectByUserId(const std::string& userId, int offset, int limit);
	bool IsReadyForCreationByTime(const std::string& userId);
	int64_t GetCountSpacesWithUser(const std::string& userId);
	void CreateSpaceAndItsOwner(const std::string& name, const std::string& key, bool requestsAllowed, const std::string& userId);
	PagingResult<model::SpaceInvitation> SelectInvitations(const std::string& userId, int start, int limit);
	PagingResult<model::SpaceInvitation> SelectInvitationsBySpace(const boost::uuids::uuid& spaceId, const std::string& userId, int start, int limit);
	model::Space SelectByLink(const boost::uuids::uuid& link);
	PagingResult<model::SpaceLink> SelectSpaceLinkList(const std::string& userId, int offset, int limit);
	void CreateInvitation(const boost::uuids::uuid& spaceId, const std::string& userId, const Role::Type& role, const std::string& creatorId);

private:
	storages::postgres::ClusterPtr _pg;
	table::Space _space;
	table::SpaceUser _spaceUser;
	table::SpaceInvitation _spaceInvitation;
	table::SpaceLink _spaceLink;
};

} // namespace svetit::space
