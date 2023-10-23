#include "invitation_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::space {

formats::json::Value Serialize(
	const std::vector<svetit::space::SpaceInvitation> si,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["items"];

	int i = 0;
	for (auto inv: si) {
		builder["items"][std::to_string(i)]["id"] = inv.id;
		builder["items"][std::to_string(i)]["spaceId"] = inv.spaceId;
		builder["items"][std::to_string(i)]["creatorId"] = inv.creatorId;
		builder["items"][std::to_string(i)]["userId"] = inv.userId;
		builder["items"][std::to_string(i)]["role"] = inv.role;
		builder["items"][std::to_string(i)]["createdAt"] = date::format("%F %T", inv.createdAt);
		i++;
    }

	return builder.ExtractValue();
}

} // namespace svetit::space
