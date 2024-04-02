#include "control_circuit_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ControlCircuit& controlCircuit,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = controlCircuit.id;
	builder["typeId"] = controlCircuit.typeId;
	builder["sectionId"] = controlCircuit.sectionId;
	builder["name"] = controlCircuit.name;
	builder["isDeleted"] = controlCircuit.isDeleted;

	return builder.ExtractValue();
}

ControlCircuit Parse(
	const formats::json::Value& json,
	formats::parse::To<ControlCircuit>)
{
	return {
		.id = json["id"].As<int>(),
		.typeId = json["typeId"].As<int>(),
		.sectionId = json["sectionId"].As<int>(),
		.name = json["name"].As<std::string>(),
		.isDeleted = json["isDeleted"].As<bool>(),
	};
}

} // namespace svetit::project::model