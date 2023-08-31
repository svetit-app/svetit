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

	model::Session Create(
		const OIDCTokens& tokens,
		const TokenPayload& data,
		const std::string& userAgent);

private:
	table::Session& _table;
	tokens::Session& _tokenizer;
};

} // namespace svetit::auth::service

