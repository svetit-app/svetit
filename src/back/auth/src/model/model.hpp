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
	std::string _userId;
};

} // namespace svetit::auth
