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
	builder["displayName"] = s._displayName;
	builder["login"] = s._login;
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
		._displayName = json["displayName"].As<std::string>(),
		._login = json["login"].As<std::string>(),
		._firstname = json["firstnae"].As<std::string>(),
		._lastname = json["lastname"].As<std::string>(),
		._email = json["email"].As<std::string>(),
	};
}

UserInfo MapFromOIDCUserInfo(const formats::json::Value& json)
{
	return {
		._id = utils::BoostUuidFromString(json["sub"].As<std::string>()),
		._displayName = json["name"].As<std::string>(),
		._login = json["preferred_username"].As<std::string>(),
		._firstname = json["given_name"].As<std::string>(),
		._lastname = json["family_name"].As<std::string>(),
		._email = json["email"].As<std::string>(),
	};
}

UserInfo MapFromOIDCUserInfoById(const formats::json::Value& json)
{
	return {
		._id = utils::BoostUuidFromString(json["id"].As<std::string>()),
		._displayName = json["firstName"].As<std::string>() + " " + json["lastName"].As<std::string>(),
		._login = json["username"].As<std::string>(),
		._firstname = json["firstName"].As<std::string>(),
		._lastname = json["lastName"].As<std::string>(),
		._email = json["email"].As<std::string>(),
	};
}

std::vector<UserInfo> MapFromOIDCUserInfoList(const formats::json::Value& json)
{
	std::vector<UserInfo> list;
	list.reserve(json.GetSize());
	for (auto&& it : json)
	{
		auto info = MapFromOIDCUserInfoById(it);
		list.push_back(std::move(info));
	}
	return list;
}

} // namespace svetit::auth::model