#pragma once

#include "../model/session.hpp"
#include "../model/oidctokens.hpp"
#include "../repo/table_session.hpp"
#include "tokens_session.hpp"

namespace svetit::auth::service {

class Session final {
public:
	explicit Session(
		table::Session& table,
		tokens::Session& tokenizer);

	table::Session& Table();
	tokens::Session& Token();

	model::Session Create(
		const OIDCTokens& tokens,
		const TokenPayload& data,
		const std::string& userAgent,
		const std::chrono::system_clock::time_point& exp);

	model::Session Refresh(
		const OIDCTokens& tokens,
		const TokenPayload& data,
		const std::string& userAgent,
		const std::chrono::system_clock::time_point& exp,
		const boost::uuids::uuid& oldSessionId);

private:
	table::Session& _table;
	tokens::Session& _tokenizer;

	model::Session prepare(
		const OIDCTokens& tokens,
		const TokenPayload& data,
		const std::string& userAgent,
		const std::chrono::system_clock::time_point& exp);
};

} // namespace svetit::auth::service

