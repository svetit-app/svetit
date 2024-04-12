#pragma once

#include <string>

namespace svetit::project::model {

struct ValueView {
	int id;
	int diTypeId;
	std::string value;
	std::string view;
};

} // namespace svetit::project::model