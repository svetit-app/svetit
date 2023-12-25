#pragma once

#include "../model/oidctokens.hpp"
#include "../model/userinfo.hpp"

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth {

class OIDConnect final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "oidc";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit OIDConnect(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	std::string GetPrivateIssuer() const;
	std::string GetJWKS() const;
	std::string GetLoginUrl(
		const std::string& state,
		const std::string& redirectUrl) const;
	std::string GetLogoutUrl(
		const std::string& idToken,
		const std::string& redirectUrl) const;

	OIDCTokens Exchange(
		const std::string& code,
		const std::string& redirectUrl) const;
	OIDCTokens Refresh(const std::string& refreshToken) const;

	model::UserInfo GetUserInfo(const std::string& token) const;

	model::UserInfo GetUserInfoById(
		const std::string& id,
		const std::string& token) const;

	std::vector<model::UserInfo> GetUserInfoList(const std::string& search, const std::string& token, unsigned int start, unsigned int limit);

private:
	std::string _clientId;
	std::string _clientSecret;
	clients::http::Client& _http;

	struct ProviderUrls {
		std::string
			_issuer,
			_auth,
			_logout,
			_token,
			_jwks,
			_userInfo,
			_userInfoById;
	} _urls;
};

} // namespace svetit::auth