#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace svetit::auth::model {

struct UserInfo {
	boost::uuids::uuid _id;
	std::string _displayName;
	std::string _login;
	std::string _firstname;
	std::string _lastname;
	std::string _email;
};

} // namespace svetit::auth::model
