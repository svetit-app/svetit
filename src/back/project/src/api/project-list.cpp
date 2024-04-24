#include "project-list.hpp"
#include "../service/service.hpp"
#include "../model/project_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/schemas.hpp>

#include <userver/formats/json/validate.hpp>
#include <userver/fs/blocking/read.hpp>

namespace svetit::project::handlers {

ProjectList::ProjectList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{}
{
	LoadSchemas(std::string(kName), _s.GetJSONSchemasPath(), server::http::HttpMethod::kGet, true, false, _mapHttpMethodToSchema);
}

formats::json::Value ProjectList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		auto jsonSchemasForMethod = _mapHttpMethodToSchema.at(req.GetMethod());
		auto schemaDocumentParams = formats::json::FromString(jsonSchemasForMethod.params);

		std::string jsonDocumentStr = GenerateJsonDocument(schemaDocumentParams, req);
		LOG_WARNING() << "JsonDocumentStr by request params and headers: " << jsonDocumentStr;
		formats::json::Value jsonDocument;

		try {
			jsonDocument = formats::json::FromString(jsonDocumentStr);
			formats::json::Schema schema(schemaDocumentParams);
			auto result = formats::json::Validate(jsonDocument, schema);
			LOG_WARNING() << "Validation result: " << result;
			if (!result)
				throw errors::BadRequest400("wrong params");
		} catch (formats::json::ParseException& e) {
			throw errors::BadRequest400("wrong params");
		}

		auto paging = parsePaging(req);
		res = _s.GetProjectList(paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
