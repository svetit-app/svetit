#include <chrono>
#include <string>

#include "session.hpp"
#include "../repo/repository.hpp"

namespace svetit::auth {

Session::Session(
	    svetit::auth::Repository& repo
        )
	: _repo{repo}
{
}

void Session::Save(const std::string& userId, const std::string& device, const std::string& accessToken, const std::string& refreshToken, const std::string& idToken){
    const std::string token = "session_token"; // todo: fix generation and usage of real session jwt token
    const std::chrono::system_clock::time_point created = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point expired = created + std::chrono::hours(24); // todo: fix hardcoded session expire
    const bool active = true;
    _repo.Session().Save(token, created, expired, userId, device, accessToken, refreshToken, idToken, active);
}

} // namespace svetit::auth