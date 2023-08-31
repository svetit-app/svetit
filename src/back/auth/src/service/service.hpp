#pragma once

#include "../model/model.hpp"
#include "../model/oidctokens.hpp"
#include "session.hpp"

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>

namespace svetit::auth {

class Tokenizer;
class OIDConnect;
class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	service::Session& Session();

	std::string GetErrorPageUrl(const std::string& url) const;

	std::string GetLoginUrl(const std::string& callbackUrl) const;
	std::string GetLoginCompleteUrl(
		const std::string& url,
		const http::Args& args) const;

	std::string GetLogoutUrl(
		const std::string& idToken,
		const std::string& callbackUrl) const;
	std::string GetLogoutCompleteUrl(const std::string& url) const;

	OIDCTokens GetTokens(
		const std::string& state,
		const std::string& code);

	TokenPayload GetOIDCTokenPayload(const OIDCTokens& tokens) const;

	OIDCTokens TokenRefresh(const std::string& refreshToken);

	std::string GetTokenUserId(const std::string& token) const;
	auto GetTokenPayload(const std::string& token) const;

private:
	std::string _webErrorPath;
	std::string _webLoginPath;
	std::string _webLogoutPath;
	Tokenizer& _tokenizer;
	OIDConnect& _oidc;
	Repository& _rep;
	service::Session _session;
};

} // namespace svetit::auth

