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
		.with_issuer(_issuer)
		.leeway(60UL); // value in seconds, add some to compensate timeout

	_jwt = std::make_shared<jwt_session_impl>(std::move(verifier), std::move(algo));
}

std::string Session::Create(const std::string& userId)
{
	std::string sessionToken = jwt::create()
		.set_issuer(_issuer)
		.set_type("JWT")
		.set_issued_at(std::chrono::system_clock::now())
		.set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{36000})
		.set_subject(userId)
		.sign(_jwt->_algo);

	return sessionToken;
}

SessionTokenPayload Session::Verify(const std::string& token)
{
	auto verify = jwt::verify()
		.allow_algorithm(_jwt->_algo)
		.with_issuer(_issuer);
	auto decoded = jwt::decode(token);
	verify.verify(decoded);

	return {
		._userId = decoded.get_subject()
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
