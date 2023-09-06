#include "userinfo_serialize.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::auth::model {

formats::json::Value Serialize(
	const UserInfo& s,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["id"] = utils::ToString(s._id);
	builder["name"] = s._name;
	builder["username"] = s._username;
	builder["firstname"] = s._firstname;
	builder["lastname"] = s._lastname;
	builder["email"] = s._email;

	return builder.ExtractValue();
}

UserInfo Parse(
	const formats::json::Value& json,
	formats::parse::To<UserInfo>)
{
	return {
		._id = utils::BoostUuidFromString(json["id"].As<std::string>()),
		._name = json["name"].As<std::string>(),
		._username = json["username"].As<std::string>(),
		._firstname = json["firstnae"].As<std::string>(),
		._lastname = json["lastname"].As<std::string>(),
		._email = json["email"].As<std::string>(),
	};
}

UserInfo MapFromOIDCUserInfo(const formats::json::Value& json)
{
	return {
		._id = utils::BoostUuidFromString(json["sub"].As<std::string>()),
		._name = json["name"].As<std::string>(),
		._username = json["preferred_username"].As<std::string>(),
		._firstname = json["given_name"].As<std::string>(),
		._lastname = json["family_name"].As<std::string>(),
		._email = json["email"].As<std::string>(),
	};
}

} // namespace svetit::auth::model
