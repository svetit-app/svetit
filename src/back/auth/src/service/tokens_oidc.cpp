#include "tokens_oidc.hpp"

#include <fmt/format.h>

#include <memory>
#include <stdexcept>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "jwt-cpp/traits/kazuho-picojson/traits.h"
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/http/url.hpp>

#include <jwt-cpp/traits/kazuho-picojson/defaults.h>

namespace svetit::auth::tokens {

struct jwt_impl {
	std::string _issuer;
	jwt::jwks<jwt::traits::kazuho_picojson> _jwks;

	using Verifier = jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson>;
	std::map<std::string, std::shared_ptr<Verifier>> _verifier;

	Verifier* GetVerifier(const std::string& keyId, const std::string& algoName)
	{
		auto it = _verifier.find(keyId);
		if (it != _verifier.end())
			return it->second.get();

		auto jwk = _jwks.get_jwk(keyId);
		auto x5c = jwk.get_x5c_key_value();
		if (x5c.empty() || _issuer.empty())
			throw std::runtime_error("fail to get x5c key");

		if (algoName != "RS256")
			LOG_ERROR() << "Wrong token algorithm" << algoName;
		// TODO: choose algorithm by token param
		auto algo = jwt::algorithm::rs256(jwt::helper::convert_base64_der_to_pem(x5c), "", "", "");

		auto verifier = jwt::verify()
			.allow_algorithm(std::move(algo))
			.with_issuer(_issuer)
			.leeway(60UL); // value in seconds, add some to compensate timeout
		auto res = _verifier.emplace(keyId, std::make_shared<Verifier>(std::move(verifier)));
		return res.first->second.get();
	}
};

OIDC::OIDC()
{
}

void OIDC::SetJWKS(const std::string& issuer, const std::string& raw)
{
	jwt_impl impl{
		._issuer = issuer,
		._jwks = jwt::parse_jwks(raw)
	};

	_jwt = std::make_shared<jwt_impl>(std::move(impl));
}

TokenPayload OIDC::Parse(const std::string& raw)
{
	auto token = jwt::decode(raw);
	auto* verifier = _jwt->GetVerifier(token.get_key_id(), token.get_algorithm());
	verifier->verify(token);

	TokenPayload tokenPayload = {
		._userId = token.get_subject(),
		._userName = token.get_payload_claim("name").as_string(),	
		._userLogin = token.get_payload_claim("preferred_username").as_string()			
	};

	return tokenPayload;
}

} // namespace svetit::auth::tokens