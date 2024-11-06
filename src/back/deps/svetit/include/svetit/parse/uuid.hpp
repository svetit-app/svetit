#pragma once

#include <userver/utils/boost_uuid4.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

USERVER_NAMESPACE_BEGIN

namespace formats::serialize {

json::Value Serialize(
	const boost::uuids::uuid& item,
	serialize::To<json::Value>);

} // namespace formats::serialize

namespace formats::parse {

boost::uuids::uuid Parse(const formats::json::Value& json,
	formats::parse::To<boost::uuids::uuid>);

} // namespace formats::parse

USERVER_NAMESPACE_END
