#include "user_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const std::vector<svetit::space::model::SpaceUser> su,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["items"];

	int i = 0;
	for (auto u: su) {
		builder["items"][std::to_string(i)]["spaceId"] = u.spaceId;
		builder["items"][std::to_string(i)]["userId"] = u.userId;
		builder["items"][std::to_string(i)]["isOwner"] = u.isOwner;
		// if (u.isOwner) {
		// 	builder["items"][std::to_string(i)]["isOwner"] = "true";
		// } else {
		// 	builder["items"][std::to_string(i)]["isOwner"] = "false";
		// }
		builder["items"][std::to_string(i)]["joinedAt"] = date::format("%F %T", u.joinedAt);
		builder["items"][std::to_string(i)]["role"] = u.role;
		i++;
    }

	return builder.ExtractValue();
}

} // namespace svetit::space::model
