#pragma once

#include <string>
#include "../repo/repository.hpp"
#include "tokenizer.hpp"

namespace svetit::auth {

class Session final {
public:
	explicit Session(Repository& repo, svetit::auth::Tokenizer& tokenizer);

	void Save(const std::string& userId, const std::string& device, const std::string& accessToken, const std::string& refreshToken, const std::string& idToken);

private:
	Repository& _repo;
	svetit::auth::Tokenizer& _tokenizer;

};

} // namespace svetit::auth

