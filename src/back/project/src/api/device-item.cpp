#include "device-item.hpp"
#include "../service/service.hpp"
#include "../model/device_item.hpp"
#include "../model/device_item_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

DeviceItem::DeviceItem(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value DeviceItem::HandleRequestJsonThrow(
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
		case server::http::HttpMethod::kPatch:
			return Patch(req, body, res);
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

formats::json::Value DeviceItem::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");
	res = _s.GetDeviceItem(id);

	return res.ExtractValue();
}

formats::json::Value DeviceItem::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto deviceItem = body.As<model::DeviceItem>();

	_s.CreateDeviceItem(deviceItem);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value DeviceItem::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto deviceItem = body.As<model::DeviceItem>();

	_s.UpdateDeviceItem(deviceItem);

	return res.ExtractValue();
}

formats::json::Value DeviceItem::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");

	_s.DeleteDeviceItem(id);

	return res.ExtractValue();
}
} // namespace svetit::project::handlers
