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

} // namespace svetit::space