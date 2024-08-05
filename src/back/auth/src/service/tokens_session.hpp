#pragma once

#include <memory>
#include <string>

#include <userver/utils/async.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/clients/http/client.hpp>

#include "../model/model.hpp"

namespace svetit::auth::tokens {

struct jwt_session_impl;

class Session final {
	static constexpr std::string_view _issuer = "svetit";
	static constexpr std::string_view _sessionIdKey = "ses";
public:
	Session(
		const components::ComponentContext& ctx,
		const std::string& privateKeyPath);
	~Session();

	std::string Create(
		const std::string& userId,
		const std::string& sessionId);
	SessionTokenPayload Verify(const std::string& token);

private:
	std::string readKey(const std::string& path) const;
	std::shared_ptr<jwt_session_impl> _jwt;
	engine::TaskWithResult<void> _task;
};

} // namespace svetit::auth::tokens
