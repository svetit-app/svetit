#pragma once

#include <string>
#include <chrono>

namespace svetit::space {

struct SpaceTokenPayload {
	std::string
		_key,
		_id,
		_roleId,
		_userId;
};

const int GLOBAL_SPACE_ROLE_OPERATOR = 1;
const int GLOBAL_SPACE_ROLE_USER = 2;
const int GLOBAL_SPACE_ROLE_ADMIN = 3;

} // namespace svetit::space