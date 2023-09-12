#pragma once

#include "../model/session.hpp"

#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::auth::table {

class Session final {
public:
	explicit Session(storages::postgres::ClusterPtr pg);

	void Save(const model::Session& data);

	model::Session GetById(const std::string& id, bool isActive = true);

	model::Session Get(const std::string& id);

	void UpdateTokens(const model::Session& s);

	bool Refresh(
		const model::Session& data,
		const boost::uuids::uuid& oldId);

	void MarkInactive(const boost::uuids::uuid& sid);

	void MarkInactive(const std::string& userId);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::auth::table
