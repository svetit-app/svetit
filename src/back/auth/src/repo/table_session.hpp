#pragma once

#include "../model/model.hpp"

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
	static constexpr std::string_view kName = "session";

	explicit Session(
	    storages::postgres::ClusterPtr pg);

    void Save(
        const std::string& token,
        const std::chrono::system_clock::time_point& created,
        const std::chrono::system_clock::time_point& expired,
        const std::string& userId,
        const std::string& device,
        const std::string& accessToken,
        const std::string& refreshToken,
        const std::string& idToken,
        const bool& active);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::auth::table