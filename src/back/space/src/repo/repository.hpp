#pragma once

#include "table_space.hpp"
#include "table_space_user.hpp"
#include "table_space_invitation.hpp"
#include "table_space_link.hpp"
#include "table_role.hpp"

#include "../model/group.hpp"
#include <svetit/db/db_table.hpp>

#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space {

namespace table {

using Group = db::Table<model::Group>;

} // namespace table

class Repository {
public:
	Repository(storages::postgres::ClusterPtr pg);
	Repository(std::shared_ptr<db::Base> dbPtr);

	table::Space& Space();
	table::SpaceUser& SpaceUser();
	table::SpaceInvitation& SpaceInvitation();
	table::SpaceLink& SpaceLink();
	table::Group& Group();
	table::Role& Role();

	Repository WithTrx(const storages::postgres::TransactionOptions& opt = storages::postgres::Transaction::RW);
	void Commit();

	PagingResult<model::Space> SelectAvailable(const std::string& userId, int offset, int limit);
	PagingResult<model::Space> SelectAvailableBySpaceName(const std::string& spaceName, const std::string& userId, int offset, int limit);

	PagingResult<model::Space> SelectByUserId(const std::string& userId, int offset, int limit);

	bool IsReadyForCreationByTime(const std::string& userId);
	int64_t GetCountSpacesWithUser(const std::string& userId);

	PagingResult<model::SpaceInvitation> SelectInvitations(const std::string& userId, int start, int limit);
	PagingResult<model::SpaceInvitation> SelectInvitationsBySpace(const boost::uuids::uuid& spaceId, const std::string& userId, int start, int limit);

	model::Space SelectByLink(const boost::uuids::uuid& link);

	PagingResult<model::SpaceLink> SelectSpaceLinkList(const std::string& userId, int offset, int limit);

	void CreateInvitation(const boost::uuids::uuid& spaceId, const std::string& userId, std::optional<int> roleId, const std::string& creatorId);
	int64_t GetAvailableInvitationsCount(const std::string& currentUserId);
private:
	std::shared_ptr<db::Base> _db;
	table::Space _space;
	table::SpaceUser _spaceUser;
	table::SpaceInvitation _spaceInvitation;
	table::SpaceLink _spaceLink;
	table::Group _group;
	table::Role _role;
};

class RepositoryComponent final : public components::LoggableComponentBase, public Repository {
public:
	static constexpr std::string_view kName = "repository";
	static yaml_config::Schema GetStaticConfigSchema();
	explicit RepositoryComponent(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);
};

} // namespace svetit::space
