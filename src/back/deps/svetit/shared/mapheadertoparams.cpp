#include "mapheadertoparams.hpp"
#include "headers.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit {

void MapHeaderToParams(formats::json::Value& params)
{
	formats::json::ValueBuilder paramsBuilder(std::move(params));
	if (paramsBuilder.HasMember(headers::kSpaceId) && !paramsBuilder.HasMember("spaceId"))
		paramsBuilder["spaceId"] = paramsBuilder[std::string(headers::kSpaceId)];
	params = paramsBuilder.ExtractValue();
}

} // namespace svetit

