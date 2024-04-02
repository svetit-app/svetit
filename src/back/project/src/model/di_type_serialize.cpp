#include "di_type_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DiType& diType,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["id"] = diType.id;
	builder["measureId"] = diType.measureId;;
	builder["saveTimerId"] = diType.saveTimerId;
	builder["key"] = diType.key;
	builder["name"] = diType.name;
	builder["mode"] = DiMode::ToString(diType.mode);
	builder["saveAlgorithm"] = SaveAlgorithm::ToString(diType.saveAlgorithm);
	builder["isDeleted"] = diType.isDeleted;

	return builder.ExtractValue();
}

DiType Parse(
	const formats::json::Value& json,
	formats::parse::To<DiType>)
{
	return {
		.id = json["id"].As<int>(),
		.measureId = json["measureId"].As<int>(),
		.saveTimerId = json["saveTimerId"].As<int>(),
		.key = json["key"].As<std::string>(),
		.name = json["name"].As<std::string>(),
		.mode = DiMode::FromString(json["mode"].As<std::string>()),
		.saveAlgorithm = SaveAlgorithm::FromString(json["saveAlgorithm"].As<std::string>()),
		.isDeleted = json["isDeleted"].As<bool>(),
	};
}

} // namespace svetit::project::model