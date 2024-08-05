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
		return "byTimer";
	case ByTimerOrImmediately:
		return "byTimerOrImmediately";
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
	if (saveAlgorithm == "byTimer")
		return ByTimer;
	if (saveAlgorithm == "byTimerOrImmediately")
		return ByTimerOrImmediately;

	throw errors::BadRequest400("Wrong save algorithm");
}

formats::json::Value Serialize(
	const SaveAlgorithm::Type& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder res{SaveAlgorithm::ToString(item)};
	return res.ExtractValue();
}

SaveAlgorithm::Type Parse(
	const formats::json::Value& json,
	formats::parse::To<SaveAlgorithm::Type>)
{
	return SaveAlgorithm::FromString(json.As<std::string>());
}

} // namespace svetit::project
