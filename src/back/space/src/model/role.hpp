#pragma once

#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space {

struct Role {
	int id;
	boost::uuids::uuid spaceId;
	std::string name;
};

} // namespace svetit::space