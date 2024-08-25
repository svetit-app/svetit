#include "tokens.hpp"

#include <exception>
#include <stdexcept>
#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/fs/blocking/read.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/http/url.hpp>

#include <jwt-cpp/jwt.h>

namespace svetit::space::tokens {

/*static*/ yaml_config::Schema Tokens::GetStaticConfigSchema()
{
return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Tokens
additionalProperties: false
properties:
  internalTlsKeyPath:
    type: string
    description: TLS Private Key path
)");
}

struct jwt_session_impl {
	using Verifier = jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson>;

	jwt_session_impl(Verifier&& verifier, jwt::algorithm::rs256&& algo)
		: _verifier{std::move(verifier)}
		, _algo{std::move(algo)}
	{}

	Verifier _verifier;
	jwt::algorithm::rs256 _algo;
};

Tokens::Tokens(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _privateKeyPath{conf["internalTlsKeyPath"].As<std::string>()}
{
	const auto key = readKey(_privateKeyPath);
	jwt::algorithm::rs256 algo{"", key, "", ""};

	auto verifier = jwt::verify()
		.allow_algorithm(algo)
		.with_issuer(std::string{_issuer})
		.leeway(60UL); // value in seconds, add some to compensate timeout

	_jwt = std::make_shared<jwt_session_impl>(std::move(verifier), std::move(algo));
}

std::string Tokens::Create(
		const std::string& key,
		const std::string& id,
		const std::string& roleId,
		const std::string& userId,
		int secs)
{
	std::string token = jwt::create()
		.set_issuer(std::string{_issuer})
		.set_type("JWT")
		.set_issued_at(std::chrono::system_clock::now())
		.set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{secs})
		.set_subject(userId)
		.set_payload_claim("key", jwt::claim(key))
		.set_payload_claim("id", jwt::claim(id))
		.set_payload_claim("roleId", jwt::claim(roleId))
		.sign(_jwt->_algo);

	return token;
}

SpaceTokenPayload Tokens::Verify(const std::string& token)
{
	auto verify = jwt::verify()
		.allow_algorithm(_jwt->_algo)
		.with_issuer(std::string{_issuer});
	auto decoded = jwt::decode(token);
	verify.verify(decoded);

	return {
		._key = decoded.get_payload_claim("key").as_string(),
		._id = decoded.get_payload_claim("id").as_string(),
		._roleId = decoded.get_payload_claim("roleId").as_string(),
		._userId = decoded.get_subject()
	};
}

std::string Tokens::readKey(const std::string& path) const
{
	try {
		return fs::blocking::ReadFileContents(path);
	} catch (const std::exception& e) {
		const auto msg = fmt::format("[tokenizer][session] Failed read TLS private key: '{}'.", e.what());
		throw std::runtime_error(msg);
	}
	return {};
}

} // namespace svetit::auth::tokens