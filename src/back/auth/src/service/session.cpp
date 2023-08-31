#include "session.hpp"
#include "../repo/repository.hpp"
#include "tokenizer.hpp"

namespace svetit::auth::service {

Session::Session(
	table::Session& table,
	tokens::Session& tokenizer)
	: _table{table}
	, _tokenizer(tokenizer)
{
}

model::Session Session::Create(
	const OIDCTokens& tokens,
	const TokenPayload& data,
	const std::string& userAgent)
{
	auto sessionToken = _tokenizer.Create(data._userId);
	
	auto now = std::chrono::system_clock::now();

	model::Session session{
		._created = now,
		._expired = now + std::chrono::hours(24),
		._token = std::move(sessionToken),
		._userId = data._userId,
		._device = userAgent, // todo: is user-agent enough for device detection?
		._accessToken = tokens._accessToken,
		._refreshToken = tokens._refreshToken,
		._idToken = tokens._idToken
	};

	_table.Save(session);
	return session;
}

} // namespace svetit::auth::service
