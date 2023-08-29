#pragma once

#include <string>

namespace svetit::auth::tokens {

class InternalTokens final {

public:
	InternalTokens();

    std::string CreateSignedSessionToken(const std::string& userId, const std::string& privateKey);

};

} // namespace svetit::auth::tokens
