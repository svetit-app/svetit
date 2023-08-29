#include "tokens_internal.hpp"

#include <jwt-cpp/jwt.h>

namespace svetit::auth::tokens {

InternalTokens::InternalTokens()
{
}

std::string InternalTokens::CreateSignedSessionToken(const std::string& userId, const std::string& privateKey)
{

    std::string sessionToken = jwt::create()
        .set_issuer("svetit")
        .set_type("JWT")
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{36000})
        .set_subject(userId)
        .sign(jwt::algorithm::rs256("", privateKey, "", ""));

    return sessionToken;   
}

} // namespace svetit::auth::tokens