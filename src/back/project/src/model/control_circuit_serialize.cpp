#include "control_circuit_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ControlCircuit& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = item.id;
	builder["typeId"] = item.typeId;
	builder["sectionId"] = item.sectionId;
	builder["name"] = item.name;

	return builder.ExtractValue();
}

ControlCircuit Parse(
	const formats::json::Value& json,
	formats::parse::To<ControlCircuit>)
{
	return {
		.id = json["id"].As<int64_t>(),
		.spaceId = {},
		.typeId = json["typeId"].As<int64_t>(),
		.sectionId = json["sectionId"].As<int64_t>(),
		.name = json["name"].As<std::string>()
	};
}

} // namespace svetit::project::model