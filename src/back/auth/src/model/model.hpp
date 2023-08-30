#pragma once

#include <string>
#include <chrono>

namespace svetit::auth {

struct Tokens {
	std::string
		_accessToken,
		_refreshToken,
		_logoutToken;
};

struct TokensAndUserData {
	std::string
		_accessToken,
		_refreshToken,
		_logoutToken,
		_sessionToken,
		_userId,
		_userLogin,
		_userName;
};

struct TokenPayload {
	std::string
		_userId,
		_userName,
		_userLogin;
};

struct SessionData {
	std::string _token;
	std::chrono::system_clock::time_point _created;
	std::chrono::system_clock::time_point _expired;
	std::string _userId;
	std::string _device;
	std::string _accessToken;
	std::string _refreshToken;
	std::string _idToken;
	bool _active;
};

} // namespace svetit::auth


