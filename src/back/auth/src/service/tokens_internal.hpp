#pragma once

#include <string>

namespace svetit::auth::tokens {

class Internal final {

public:
	Internal();

	std::string CreateSignedSessionToken(const std::string& userId, const std::string& privateKey);

};

} // namespace svetit::auth::tokens
