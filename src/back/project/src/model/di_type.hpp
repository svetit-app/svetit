#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

#include "di_mode.hpp"
#include "save_algorithm.hpp"

namespace svetit::project::model {

struct DiType {
	int id;
	int measureId;
	int saveTimerId;
	std::string key;
	std::string name;
	DiMode::Type mode;
	SaveAlgorithm::Type saveAlgorithm;
	bool isDeleted;
};

} // namespace svetit::project::model