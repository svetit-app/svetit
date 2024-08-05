#include "tokens_oidc.hpp"

#include <fmt/format.h>

#include <memory>
#include <stdexcept>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/http/url.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/engine/shared_mutex.hpp>

#include <jwt-cpp/traits/kazuho-picojson/traits.h>
#include <jwt-cpp/traits/kazuho-picojson/defaults.h>

namespace svetit::auth::tokens {

struct jwt_impl {
	std::string _issuer;
	jwt::jwks<jwt::traits::kazuho_picojson> _jwks;

	userver::engine::SharedMutex _mutex;

	jwt_impl(const std::string& issuer, const std::string& raw)
	: _issuer{issuer}
	, _jwks{jwt::parse_jwks(raw)}
	{}

	using Verifier = jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson>;
	std::map<std::string, std::shared_ptr<Verifier>> _verifier;

	std::shared_ptr<Verifier> GetVerifier(const std::string& keyId, const std::string& algoName)
	{
		{
			std::shared_lock<engine::SharedMutex> lock(_mutex);
			auto it = _verifier.find(keyId);
			if (it != _verifier.end())
				return it->second;
		}

		std::lock_guard<engine::SharedMutex> lock(_mutex);

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
		return res.first->second;
	}
};

OIDC::OIDC()
{
}

void OIDC::SetJWKS(const std::string& issuer, const std::string& raw)
{
	_jwt = std::make_shared<jwt_impl>(issuer, raw);
}

void OIDC::Verify(const std::string& raw)
{
	auto token = jwt::decode(raw);
	auto verifier = _jwt->GetVerifier(token.get_key_id(), token.get_algorithm());
	verifier->verify(token);
}

TokenPayload OIDC::Parse(const std::string& raw) const
{
	auto token = jwt::decode(raw);
	return {
		._userId = token.get_subject(),
		._userName = token.get_payload_claim("name").as_string(),
		._userLogin = token.get_payload_claim("preferred_username").as_string()
	};
}

} // namespace svetit::auth::tokens
