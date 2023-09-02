#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace svetit::auth::model {

struct UserInfo {
	boost::uuids::uuid _id;
	std::string _name;
	std::string _username;
	std::string _firstname;
	std::string _lastname;
	std::string _email;
};

} // namespace svetit::auth::model
