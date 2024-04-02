#include "di_mode.hpp"
#include <shared/errors.hpp>

namespace svetit::project {

/*static*/ std::string DiMode::ToString(const DiMode::Type& diMode)
{
	switch (diMode) {
	case ReadonlyFlag:
		return "readonly_flag";
	case ReadwriteFlag:
		return "readwrite_flag";
	case Readonly:
		return "readonly";
	case Readwrite:
		return "readwrite";
	case File:
		return "file";
	case Button:
		return "button";
	case VideoStream:
		return "video_stream";
	default:
		break;
	}

	return {};
}

/*static*/ DiMode::Type DiMode::FromString(const std::string& diMode)
{
	if (diMode == "readonly_flag")
		return ReadonlyFlag;
	if (diMode == "readwrite_flag")
		return ReadwriteFlag;
	if (diMode == "readonly")
		return Readonly;
	if (diMode == "readwrite")
		return Readwrite;
	if (diMode == "file")
		return File;
	if (diMode == "button")
		return Button;
	if (diMode == "video_stream")
		return VideoStream;

	throw errors::BadRequest400("Wrong di mode");
}

} // namespace svetit::project
