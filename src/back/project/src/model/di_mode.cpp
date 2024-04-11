#include "di_mode.hpp"
#include <shared/errors.hpp>

namespace svetit::project {

/*static*/ std::string DiMode::ToString(const DiMode::Type& diMode)
{
	switch (diMode) {
	case ReadonlyFlag:
		return "readonlyFlag";
	case ReadwriteFlag:
		return "readwriteFlag";
	case Readonly:
		return "readonly";
	case Readwrite:
		return "readwrite";
	case File:
		return "file";
	case Button:
		return "button";
	case VideoStream:
		return "videoStream";
	default:
		break;
	}

	return {};
}

/*static*/ DiMode::Type DiMode::FromString(const std::string& diMode)
{
	if (diMode == "readonlyFlag")
		return ReadonlyFlag;
	if (diMode == "readwriteFlag")
		return ReadwriteFlag;
	if (diMode == "readonly")
		return Readonly;
	if (diMode == "readwrite")
		return Readwrite;
	if (diMode == "file")
		return File;
	if (diMode == "button")
		return Button;
	if (diMode == "videoStream")
		return VideoStream;

	throw errors::BadRequest400("Wrong di mode");
}

} // namespace svetit::project
