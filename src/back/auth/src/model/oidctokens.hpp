#pragma once

#include <string>

namespace svetit::auth {

struct OIDCTokens {
	std::string _accessToken;
	std::string _refreshToken;
	std::string _idToken;
};

} // namespace svetit::auth

