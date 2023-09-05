#pragma once

#include <string>
#include <chrono>

#include <boost/uuid/uuid.hpp>

namespace svetit::auth::model {

struct Session {
	boost::uuids::uuid _id;
	std::chrono::system_clock::time_point _created;
	std::chrono::system_clock::time_point _expired;
	std::string _token;
	std::string _userId;
	std::string _device;
	std::string _accessToken;
	std::string _refreshToken;
	std::string _idToken;
	bool _active = true;
};

struct SessionRefresh {
	std::string _token;
};

} // namespace svetit::auth::model
