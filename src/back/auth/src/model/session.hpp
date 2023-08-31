#pragma once

#include <string>
#include <chrono>

namespace svetit::auth::model {

struct Session {
	bool _active = true;
	int64_t _id = 0;
	std::chrono::system_clock::time_point _created;
	std::chrono::system_clock::time_point _expired;
	std::string _token;
	std::string _userId;
	std::string _device;
	std::string _accessToken;
	std::string _refreshToken;
	std::string _idToken;
};

} // namespace svetit::auth::model
