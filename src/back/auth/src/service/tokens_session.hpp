#pragma once

#include <memory>
#include <string>

#include "../model/model.hpp"

namespace svetit::auth::tokens {

struct jwt_session_impl;

class Session final {
	static constexpr const char _issuer[] = "svetit";
public:
	Session(const std::string& privateKeyPath);

	std::string Create(const std::string& userId);
	SessionTokenPayload Verify(const std::string& token);

private:
	std::string readKey(const std::string& path) const;

	std::shared_ptr<jwt_session_impl> _jwt;
};

} // namespace svetit::auth::tokens
