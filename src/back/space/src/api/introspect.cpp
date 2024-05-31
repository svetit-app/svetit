#include "introspect.hpp"
#include "../service/service.hpp"
#include "../model/model.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>

#include <boost/uuid/uuid_io.hpp>

#include <userver/utils/boost_uuid4.hpp>
#include <userver/http/common_headers.hpp>

namespace svetit::space::handlers {

Introspect::Introspect(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

std::string Introspect::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	try {
		// проверяем и получаем заголовок с ключом Пространства
		if (!req.HasHeader("X-Original-URI"))
			throw errors::BadRequest400("Header X-Original-URI is missing");
		const std::string header = req.GetHeader("X-Original-URI");

		// извлекаем ключ Пространства из заголовка
		const std::string spaceKey = _s.GetKeyFromHeader(header);

		// генерируем имя куки и проверяем наличие
		const std::string cookieName = _s.GenerateCookieName(spaceKey);

		if (req.HasCookie(cookieName)) {
			// извлекаем токен из куки
			const std::string token = req.GetCookie(cookieName);
			LOG_DEBUG() << "Cookie exists, token: " << token;

			try {
				// проверяем и распарсиваем токен
				SpaceTokenPayload data = _s.Tokens().Verify(token);
				LOG_DEBUG() << "Token verify OK";

				// авторизуем и отдаём заголовки
				req.GetHttpResponse().SetHeader(headers::kSpaceId, data._id);
				req.GetHttpResponse().SetHeader(headers::kSpaceRole, data._role);
				return "Ok";
			} catch(const std::exception& e) {
				LOG_DEBUG() << "Fail to verify token: " << e.what();
				// TODO - может надо ловить тут разные исключения? если исключение верификации, то переходим к созданию. если исключения другие, то re-throw
			}
		}

		LOG_DEBUG() << "Creating new token";
		// получаем userId
		if (!req.HasHeader(headers::kUserId))
			throw errors::Unauthorized401();
		std::string userId = req.GetHeader(headers::kUserId);

		// Проверяем права и создаём токен
		LOG_DEBUG() << "Checking rights";
		model::Space space = _s.GetByKeyIfAdmin(spaceKey, userId);
		const std::string spaceIdStr = boost::uuids::to_string(space.id);
		const std::string role = "admin";
		const std::string token = _s.CreateToken(spaceIdStr, space.key, userId, role);
		LOG_DEBUG() << "Token created: " << token;

		// Создаём куки
		LOG_DEBUG() << "Creating cookie";
		server::http::Cookie cookie{cookieName, token};
		// todo - какие тут нам нужны настройки у создаваемой куки?
		cookie.SetPath("/");
		cookie.SetSecure();
		cookie.SetHttpOnly();
		cookie.SetSameSite("Lax");

		// Возвращаем куки и заголовки в запросе
		LOG_DEBUG() << "Setting Cookie and headers and returning OK";
		auto& resp = req.GetHttpResponse();
		resp.SetCookie(cookie);
		req.GetHttpResponse().SetHeader(headers::kSpaceId, spaceIdStr);
		req.GetHttpResponse().SetHeader(headers::kSpaceRole, role);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to verify: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Invalid space authorization token";
	}
	return "Ok";
}

} // namespace svetit::space::handlers