#pragma once

#include <string>

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
	std::string _userId;
};

} // namespace svetit::auth


