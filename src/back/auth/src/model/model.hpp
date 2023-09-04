#pragma once

#include <string>
#include <chrono>

namespace svetit::auth {

struct TokenPayload {
	std::string
		_userId,
		_userName,
		_userLogin;
};

struct SessionTokenPayload {
	std::string
		_userId,
		_sessionId;
};

struct LoginCompletePayload {
	std::string
		_url,
		_token;
};

} // namespace svetit::auth
