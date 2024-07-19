#pragma once

#include <memory>
#include <string>

#include "../model/model.hpp"

namespace svetit::auth::tokens {

struct jwt_session_impl;

class Session final {
	static constexpr std::string_view _issuer = "svetit";
	static constexpr std::string_view _sessionIdKey = "ses";
public:
	Session(const std::string& privateKeyPath);

	std::string Create(
		const std::string& userId,
		const std::string& sessionId);
	SessionTokenPayload Verify(const std::string& token);

private:
	std::string readKey(const std::string& path) const;
	void watchKey(const std::string& path);

	std::shared_ptr<jwt_session_impl> _jwt;
};

} // namespace svetit::auth::tokens
