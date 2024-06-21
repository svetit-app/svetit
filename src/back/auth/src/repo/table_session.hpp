#pragma once

#include "../model/session.hpp"
#include <shared/db/db_base.hpp>

#include <optional>
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
	explicit Session(std::shared_ptr<db::Base> dbPtr);

	void Save(const model::Session& data);

	model::Session Get(const std::string& id, const std::optional<bool>& isActive = true);

	void UpdateTokens(const model::Session& s);

	void MarkInactive(const boost::uuids::uuid& id);
	void MarkInactive(const std::string& userId);

	bool Refresh(
		const model::Session& data,
		const boost::uuids::uuid& oldId);
private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::auth::table
