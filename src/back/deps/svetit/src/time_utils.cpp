#include <svetit/time_utils.hpp>

#include <chrono>

namespace svetit {

int64_t current_unixtime()
{
	const auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

} // svetit
