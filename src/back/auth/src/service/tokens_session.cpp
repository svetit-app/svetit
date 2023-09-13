#include "tokens_session.hpp"

#include <jwt-cpp/jwt.h>

#include <memory>
#include <fstream>

namespace svetit::auth::tokens {

struct jwt_session_impl {
	using Verifier = jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson>;

	jwt_session_impl(Verifier&& verifier, jwt::algorithm::rs256&& algo)
		: _verifier{std::move(verifier)}
		, _algo{std::move(algo)}
	{}

	Verifier _verifier;
	jwt::algorithm::rs256 _algo;
};

Session::Session(const std::string& privateKeyPath)
{
	const auto key = readKey(privateKeyPath);
	jwt::algorithm::rs256 algo{"", key, "", ""};

	auto verifier = jwt::verify()
		.allow_algorithm(algo)
		.with_issuer(std::string{_issuer})
		.leeway(60UL); // value in seconds, add some to compensate timeout

	_jwt = std::make_shared<jwt_session_impl>(std::move(verifier), std::move(algo));
}

std::string Session::Create(
		const std::string& userId,
		const std::string& sessionId)
{
	std::string token = jwt::create()
		.set_issuer(std::string{_issuer})
		.set_type("JWT")
		.set_issued_at(std::chrono::system_clock::now())
		.set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{36000})
		.set_subject(userId)
		.set_payload_claim(std::string{_sessionIdKey}, jwt::claim(sessionId))
		.sign(_jwt->_algo);

	return token;
}

SessionTokenPayload Session::Verify(const std::string& token)
{
	auto verify = jwt::verify()
		.allow_algorithm(_jwt->_algo)
		.with_issuer(std::string{_issuer});
	auto decoded = jwt::decode(token);
	verify.verify(decoded);

	return {
		._userId = decoded.get_subject(),
		._sessionId = decoded.get_payload_claim(std::string{_sessionIdKey}).as_string()
	};
}

std::string Session::readKey(const std::string& path) const
{
	constexpr std::size_t read_size{4096};
	auto stream = std::ifstream(path.data());
	stream.exceptions(std::ios_base::badbit);

	if (!stream) {
		throw std::ios_base::failure("file does not exist");
	}

	std::string out;
	std::string buf(read_size, '\0');
	while (stream.read(&buf[0], read_size)) {
		out.append(buf, 0, stream.gcount());
	}
	out.append(buf, 0, stream.gcount());
	return out;
}

} // namespace svetit::auth::tokens
