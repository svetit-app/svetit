#pragma once

#include "../model/model.hpp"
#include "../model/userinfo.hpp"
#include "../model/oidctokens.hpp"
#include "../model/session_refresh.hpp"
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

	std::string GetErrorPageUrl(const std::string& url, bool forceLogout = false) const;

	std::string GetLoginUrl(const std::string& callbackUrl) const;
	LoginCompletePayload GetLoginCompleteUrl(
		const std::string& url,
		const std::string& state,
		const std::string& code,
		const std::string& userAgent,
		const std::string& redirectPath);

	std::string GetLogoutUrl(
		const std::string& sessionId,
		const std::string& callbackUrl);
	std::string GetLogoutCompleteUrl(const std::string& url) const;

	OIDCTokens TokenRefresh(const std::string& refreshToken);

	model::UserInfo GetUserInfo(const std::string& sessionId);
	model::SessionRefresh RefreshSession(
		const std::string& sessionId,
		const std::string& userAgent);

	model::UserInfo GetUserInfoById(const std::string& id, const std::string& sessionId);
	std::map<std::string, std::vector<model::UserInfo>> GetUserInfoList(const std::string& search, const std::string& sessionId, uint32_t start, uint32_t limit);
	void IntrospectUserAgentCheck(const std::string& sessionId, const std::string& userAgent);

	const std::string& GetJSONSchemasPath();

private:
	OIDCTokens getTokens(
		const std::string& state,
		const std::string& code);

	void updateTokens(model::Session& session) const;
	std::string updateSession(
		const model::Session& session,
		const std::string& userAgent);

	void differentDeviceSecurityCheck(
		const std::string& currentUserAgent,
		const std::string& oldUserAgent);

	model::Session getFreshSession(const std::string& sessionId);

	std::string _webErrorPath;
	std::string _webLoginPath;
	std::string _webLogoutPath;
	uint32_t _itemsLimitForList;
	Tokenizer& _tokenizer;
	OIDConnect& _oidc;
	Repository& _rep;
	service::Session _session;
	std::string _jsonSchemasPath;
};

} // namespace svetit::auth
