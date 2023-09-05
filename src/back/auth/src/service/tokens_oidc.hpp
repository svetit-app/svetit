#pragma once

#include <map>
#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "../model/model.hpp"

namespace svetit::auth::tokens {

struct jwt_impl;

class OIDC final {
public:
	static constexpr std::string_view kName = "tokens_oidc";

	OIDC();

	void SetJWKS(
		const std::string& issuer,
		const std::string& raw);

	void Verify(const std::string& raw);
	TokenPayload Parse(const std::string& raw) const;
	std::chrono::system_clock::time_point ParseExp(const std::string& raw) const;

private:
	std::shared_ptr<jwt_impl> _jwt;
};

} // namespace svetit::auth
