#include "session.hpp"
#include "../repo/repository.hpp"
#include "../model/errors.hpp"
#include "tokenizer.hpp"

#include <userver/utils/boost_uuid4.hpp>

namespace svetit::auth::service {

Session::Session(
	Repository& rep,
	tokens::Session& tokenizer)
	: _tokenizer(tokenizer)
	, _rep(rep)
{
}

table::Session& Session::Table() {
	return _rep.Session();
}

tokens::Session& Session::Token() {
	return _tokenizer;
}

model::Session Session::Create(
	const OIDCTokens& tokens,
	const TokenPayload& data,
	const std::string& userAgent,
	int64_t exp)
{
	auto session = prepare(tokens, data, userAgent, exp);
	_rep.Session().Save(session);
	return session;
}

model::Session Session::Refresh(
	const OIDCTokens& tokens,
	const TokenPayload& data,
	const std::string& userAgent,
	int64_t exp,
	const boost::uuids::uuid& oldSessionId)
{
	auto session = prepare(tokens, data, userAgent, exp);

	auto trx = _rep.WithTrx();
	if (!trx.Session().Refresh(session, oldSessionId)) {
		throw errors::SecurityRisk{"Same inactive session."};
	}
	trx.Commit();
	return session;
}

model::Session Session::prepare(
	const OIDCTokens& tokens,
	const TokenPayload& data,
	const std::string& userAgent,
	int64_t exp)
{
	auto id = utils::generators::GenerateBoostUuid();
	auto token = _tokenizer.Create(data._userId, utils::ToString(id));

	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

	return {
		._id = std::move(id),
		._createdAt = now,
		._expiredAt = exp,
		._token = std::move(token),
		._userId = data._userId,
		._device = userAgent, // todo: is user-agent enough for device detection?
		._accessToken = tokens._accessToken,
		._refreshToken = tokens._refreshToken,
		._idToken = tokens._idToken,
		._active = true
	};
}

} // namespace svetit::auth::service
