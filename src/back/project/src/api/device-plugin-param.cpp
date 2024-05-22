#include "device-plugin-param.hpp"
#include "../service/service.hpp"
#include "../model/device_plugin_param.hpp"
#include "../model/device_plugin_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

DevicePluginParam::DevicePluginParam(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value DevicePluginParam::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return Get(req, res);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res);
		case server::http::HttpMethod::kDelete:
			return Delete(req, res);
		default:
			throw std::runtime_error("Unsupported");
			break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value DevicePluginParam::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto deviceId = parsePositiveInt(req, "deviceId");
	const auto paramId = parsePositiveInt(req, "paramId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	res = _s.GetDevicePluginParam(spaceId, deviceId, paramId);

	return res.ExtractValue();
}

formats::json::Value DevicePluginParam::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	auto devicePluginParam = body.As<model::DevicePluginParam>();
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	devicePluginParam.spaceId = spaceId;

	_s.CreateDevicePluginParam(devicePluginParam);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value DevicePluginParam::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto deviceId = parsePositiveInt(req, "deviceId");
	const auto paramId = parsePositiveInt(req, "paramId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	_s.DeleteDevicePluginParam(spaceId, deviceId, paramId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
