#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct SectionParam {
	boost::uuids::uuid spaceId;
	int64_t sectionId;
	int64_t paramId;
};

} // namespace svetit::project::model