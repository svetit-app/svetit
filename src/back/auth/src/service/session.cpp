#include <chrono>
#include <string>

#include "session.hpp"
#include "../repo/repository.hpp"
#include "tokenizer.hpp"

namespace svetit::auth {

Session::Session(
	    svetit::auth::Repository& repo,
        svetit::auth::Tokenizer& tokenizer
        )
	: _repo{repo}
    , _tokenizer(tokenizer)
{
}

std::string Session::Save(const std::string& userId, const std::string& device, const std::string& accessToken, const std::string& refreshToken, const std::string& idToken){
    // todo: need to get key from somewhere.
    std::string privateKey = R"(-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCgN/uhZMii+zBD
1UAcgYsD1Yi77whOvU2C2Ta5GTvCD1SnMegPBrng2kqtAqJsbum57MoUpnojU/zb
1WpTwIr+mgUNBSzQHEYSDS7HUFHBtHYwrjc6j10prmdfayjKhonS5B1moOejP6ml
pKP2wbfOfiLq1DJsiQbNXKWNVTxLS/FHn+PsLpQthUsSPHhoEDmgfbuHrK3RVRMC
UKkOA5QNK0LEx+iW1Z8GNSOJjHJBcojaCBS+K+fc3LYxGwtyYjVOKJXtgJxVfMjS
0iOQO7Yi3xowq67muJiLa09SkDifDj5p2IBjTkZnzsqPDARjLRCtCY4hu/1eAJaB
ukRLjXn/AgMBAAECggEAE0dOjhfa8+WGuWV5E+pbMwhoLg5FRng30fACH4P+RqEN
SMWpYdl811CySJ7MXYDREJACAY8bRjyjwe4Bn8yczPr1HaEgSHWxJsM9puUEGWtL
XMXccRLxclkBKPWeJb8flBKNHnLBEfR7R3+yl7gCk88Qs4dPU/Opr7SfWmIbDFze
ZpE34+qe4ncPNnGmuZ8gyo+Y89B0/qnC3YBn/8CkP6esyt9X7uvZgc0O3Z8oV+Rk
u1UTN7WEu8p5XODVNuUFf5S76JoYQ1+Q6Ex25pFL90pd5SZZDcP5NjMrbe8J42sv
JPD9OefftShggm6OHmeUnOy71O1akzf2exPo4NfkNQKBgQDKUJjIgZTY7271N6gh
VzjcpqrSuX/7mf+j8PLyrdcjcFN0kEPxc5eTcSxGXiL1BbQet6uzWs4hWkuHkVbt
OA4H9rrIUPI4alYadNCYAB0CgA20S2M7HP1Wkyi3TnSUzp2YrFTD9x8OnAizHdDF
wWhrlQRm21BdderqHNSc94PvtQKBgQDKu8MemrBodrtpKJZ0nsJeIKoUuyYNg+U9
ZrxU/zZIGYVL3nNv4xXzM+hlL1Qbm/k3wTRcxkj6qb5Zi5r+stize1u0iftQfCM8
fu7xUW1LUV4cB8MPOrVNKfnglZfbB70mh2N5EJB9kF/DC0MdiA/DrI+S7GpU2WCo
OKXVDi8LYwKBgH6AGqf0bi+etpBZdqUyNnmxNMqB9oZJalEFzcLz87tS9rvBoNoO
Xs5wSzDMl6sBH/i8+30v4Ad2wBdKR0IYdJ2CJpQuGOYxhxkPxYXlPD5d9zOul6NU
owtJBIJpaffJoTzfNDk8AY1O/xmQIQQlxLbP/Gqb4wotOu+bi02CblqpAoGAJ+ur
M1aQkDF5G+7zGYm9il1yQFzN1HF5bYrSYQZUQQRNfDAUmR6lTtuU0n/sVvdyrqjr
z6z35+wO4IEeBzNeepE68KUPCL05DwuLr5cyJ1ehaXGZ1mDgHI68dp3eZjHy7ChS
zR8W5LLOOJD6KVGGkShkqTDGi2i2oMlu1jJRzzMCgYEAv2lh+RscWkbHtHdEx5be
iuXkIqd4Yz0E70UXpfKQsYY75XOlPbP3LVRqv1gPpFUYwUjUeUmO5h1zwhYRBYq+
9fYQ36S739T5utW3gI1eDXesFrp0t++utG5DMOx9O/T9OsaSCdko5Jt91FyfRMq2
0EkBHzfU5APKZDmiYH5SlPM=
-----END PRIVATE KEY-----)";

    const std::string token = _tokenizer.InternalTokens().CreateSignedSessionToken(userId, privateKey);
    const std::chrono::system_clock::time_point created = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point expired = created + std::chrono::hours(24); // todo: fix hardcoded session expire
    const bool active = true;
    _repo.Session().Save(token, created, expired, userId, device, accessToken, refreshToken, idToken, active);

    return token;
}

} // namespace svetit::auth