#pragma once

#include <string>

namespace svetit::auth {

struct Tokens {
	std::string
		_accessToken,
		_refreshToken,
		_logoutToken;
};

struct TokenPayload {
	std::string _userId;
};

} // namespace svetit::auth


