#include "session.hpp"
#include "../repo/repository.hpp"
#include "tokenizer.hpp"

#include <userver/utils/boost_uuid4.hpp>

namespace svetit::auth::service {

Session::Session(
	table::Session& table,
	tokens::Session& tokenizer)
	: _table{table}
	, _tokenizer(tokenizer)
{
}

table::Session& Session::Table() {
	return _table;
}

tokens::Session& Session::Token() {
	return _tokenizer;
}

model::Session Session::Create(
	const OIDCTokens& tokens,
	const TokenPayload& data,
	const std::string& userAgent,
	const std::chrono::system_clock::time_point& exp)
{
	auto id = utils::generators::GenerateBoostUuid();
	auto token = _tokenizer.Create(data._userId, utils::ToString(id));

	auto now = std::chrono::system_clock::now();

	model::Session session{
		._id = std::move(id),
		._created = now,
		._expired = exp,
		._token = std::move(token),
		._userId = data._userId,
		._device = userAgent, // todo: is user-agent enough for device detection?
		._accessToken = tokens._accessToken,
		._refreshToken = tokens._refreshToken,
		._idToken = tokens._idToken,
		._active = true
	};

	_table.Save(session);
	return session;
}

model::Session Session::Refresh(
	const OIDCTokens& tokens,
	const TokenPayload& data,
	const std::string& userAgent,
	const std::chrono::system_clock::time_point& exp,
	const boost::uuids::uuid& oldSessionId)
{
	auto id = utils::generators::GenerateBoostUuid();
	auto token = _tokenizer.Create(data._userId, utils::ToString(id));

	auto now = std::chrono::system_clock::now();

	model::Session session{
		._id = std::move(id),
		._created = now,
		._expired = exp,
		._token = std::move(token),
		._userId = data._userId,
		._device = userAgent, // todo: is user-agent enough for device detection?
		._accessToken = tokens._accessToken,
		._refreshToken = tokens._refreshToken,
		._idToken = tokens._idToken,
		._active = true
	};

	_table.Refresh(session, oldSessionId);
	return session;
}

} // namespace svetit::auth::service
