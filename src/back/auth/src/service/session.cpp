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

void Session::Save(const std::string& userId){
    const std::string token = "session_token";
    const std::chrono::system_clock::time_point created = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point expired = std::chrono::system_clock::now();
    const std::string device = "device";
    const std::string accessToken = "access_token";
    const std::string refreshToken = "refresh_token";
    const std::string idToken = "id_token";
    const bool active = true;
    _repo.Session().Save(token, created, expired, userId, device, accessToken, refreshToken, idToken, active);
}

} // namespace svetit::auth