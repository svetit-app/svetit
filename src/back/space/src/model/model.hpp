#pragma once

#include <string>
#include <chrono>

namespace svetit::space {

struct SpaceTokenPayload {
	std::string
		_name,
		_id,
		_role,
		_userId;
};

} // namespace svetit::space