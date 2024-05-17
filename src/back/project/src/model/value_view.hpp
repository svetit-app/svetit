#pragma once

#include <string>
#include <cstdint>

namespace svetit::project::model {

struct ValueView {
	int64_t id;
	int64_t diTypeId;
	std::string value;
	std::string view;
};

} // namespace svetit::project::model