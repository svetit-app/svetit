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

class Repository {
public:
	Repository(storages::postgres::ClusterPtr pg);
	Repository(std::shared_ptr<db::Base> dbPtr);

	table::Space& Space();
	table::SpaceUser& SpaceUser();
	table::SpaceInvitation& SpaceInvitation();
	table::SpaceLink& SpaceLink();

	Repository WithTrx(const storages::postgres::TransactionOptions& opt = storages::postgres::Transaction::RW);
	void Commit();

	std::vector<model::Space> SelectAvailable(const std::string& userId, int offset, int limit);
	int64_t SelectAvailableCount(const std::string& userId);
	std::vector<model::Space> SelectAvailableBySpaceName(const std::string& spaceName, const std::string& userId, int offset, int limit);
	int64_t SelectAvailableBySpaceNameCount(const std::string& spaceName, const std::string& userId);

	std::vector<model::Space> SelectByUserId(const std::string& userId, int offset, int limit);
	int64_t SelectByUserIdCount(const std::string& userId);

	bool IsReadyForCreationByTime(const std::string& userId);
	int64_t GetCountSpacesWithUser(const std::string& userId);

	std::vector<model::SpaceInvitation> SelectInvitations(const std::string& userId, int start, int limit);
	int64_t SelectInvitationsCount(const std::string& userId);

	std::vector<model::SpaceInvitation> SelectInvitationsBySpace(const boost::uuids::uuid& spaceId, const std::string& userId, int start, int limit);
	int64_t SelectInvitationsBySpaceCount(const boost::uuids::uuid& spaceId, const std::string& userId);

	model::Space SelectByLink(const boost::uuids::uuid& link);

	std::vector<model::SpaceLink> SelectSpaceLinkList(const std::string& userId, int offset, int limit);
	int64_t SelectSpaceLinkListCount(const std::string& userId);

	void CreateInvitation(const boost::uuids::uuid& spaceId, const std::string& userId, const Role::Type& role, const std::string& creatorId);
private:
	std::shared_ptr<db::Base> _db;
	table::Space _space;
	table::SpaceUser _spaceUser;
	table::SpaceInvitation _spaceInvitation;
	table::SpaceLink _spaceLink;
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
