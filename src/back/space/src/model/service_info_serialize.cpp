#include "service_info_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceServiceInfo& ssi,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["canCreate"] = ssi.canCreate;
	builder["invitationSize"] = ssi.invitationSize;

	return builder.ExtractValue();
}

} // namespace svetit::space::model
