#pragma once

#include <string>
#include "../repo/repository.hpp"

namespace svetit::auth {

class Session final {
public:
	explicit Session(Repository& repo);

	void Save(const std::string& userId, const std::string& device, const std::string& accessToken, const std::string& refreshToken, const std::string& idToken);

private:
	Repository& _repo;
};

} // namespace svetit::auth

