#include "save_algorithm.hpp"
#include <shared/errors.hpp>

namespace svetit::project {

/*static*/ std::string SaveAlgorithm::ToString(const SaveAlgorithm::Type& saveAlgorithm)
{
	switch (saveAlgorithm) {
	case Off:
		return "off";
	case Immediately:
		return "immediately";
	case ByTimer:
		return "by_timer";
	case ByTimerOrImmediately:
		return "by_timer_or_immediately";
	default:
		break;
	}

	return {};
}

/*static*/ SaveAlgorithm::Type SaveAlgorithm::FromString(const std::string& saveAlgorithm)
{
	if (saveAlgorithm == "off")
		return Off;
	if (saveAlgorithm == "immediately")
		return Immediately;
	if (saveAlgorithm == "by_timer")
		return ByTimer;
	if (saveAlgorithm == "by_timer_or_immediately")
		return ByTimerOrImmediately;

	throw errors::BadRequest400("Wrong save algorithm");
}

} // namespace svetit::project