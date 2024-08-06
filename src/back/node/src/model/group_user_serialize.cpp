#include "group_user_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const GroupUser& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	builder["groupId"] = item.groupId;
	builder["userId"] = item.userId;

	return builder.ExtractValue();
}

GroupUser Parse(
	const formats::json::Value& json,
	formats::parse::To<GroupUser>)
{
	return {
		.groupId = json["groupId"].As<int>(),
		.userId = json["userId"].As<std::string>()
	};
}

} // namespace svetit::node::model