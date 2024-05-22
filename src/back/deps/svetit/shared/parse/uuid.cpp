#include "uuid.hpp"

#include <userver/formats/json/value_builder.hpp>

USERVER_NAMESPACE_BEGIN

namespace formats::serialize {

formats::json::Value Serialize(
	const boost::uuids::uuid& item,
	To<formats::json::Value>)
{
	const auto str = utils::ToString(item);
	return formats::json::ValueBuilder{str}.ExtractValue();
}

} // namespace formats::serialize

namespace formats::parse {

boost::uuids::uuid Parse(
	const json::Value& json,
	To<boost::uuids::uuid>)
{
	return utils::BoostUuidFromString(json.As<std::string>());
}

} // namespace formats::parse

USERVER_NAMESPACE_END
