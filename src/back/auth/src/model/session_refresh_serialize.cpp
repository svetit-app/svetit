#include "session_refresh_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::auth::model {

formats::json::Value Serialize(
	const SessionRefresh& s,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["token"] = s._token;

	return builder.ExtractValue();
}

} // namespace svetit::auth::model
