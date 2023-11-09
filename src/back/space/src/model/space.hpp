#pragma once

#include <string>
#include <chrono>
#include <userver/utils/boost_uuid4.hpp>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct Space {
	boost::uuids::uuid id;
	std::string name;
	std::string key;
	bool requestsAllowed;
	int64_t createdAt;
};

} // namespace svetit::space::model