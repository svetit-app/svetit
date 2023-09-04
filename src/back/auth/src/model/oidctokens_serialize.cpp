#include "oidctokens_serialize.hpp"

#include <chrono>
#include <userver/formats/json/value_builder.hpp>

namespace svetit::auth {

OIDCTokens Parse(const formats::json::Value& json, formats::parse::To<OIDCTokens>) {
	return {
		._accessToken = json["access_token"].As<std::string>(),
		._refreshToken = json["refresh_token"].As<std::string>(),
		._idToken = json["id_token"].As<std::string>(),
	};
}

} // namespace svetit::auth
