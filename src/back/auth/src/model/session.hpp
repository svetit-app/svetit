#pragma once

#include <string>
#include <chrono>

#include <boost/uuid/uuid.hpp>

namespace svetit::auth::model {

struct Session {
	boost::uuids::uuid _id;
	int64_t _createdAt;
	int64_t _expiredAt;
	std::string _token;
	std::string _userId;
	std::string _device;
	std::string _accessToken;
	std::string _refreshToken;
	std::string _idToken;
	bool _active = true;
};

} // namespace svetit::auth::model
