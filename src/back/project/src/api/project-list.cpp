#include "project-list.hpp"
#include "../service/service.hpp"
#include "../model/project_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>

#include <userver/formats/json/validate.hpp>

namespace svetit::project::handlers {

ProjectList::ProjectList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value ProjectList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	//

	std::string kHandlerProjectListJsonSchema{R"(
		{
			"$schema": "http://json-schema.org/draft-07/schema#",
			"properties": {
				"limit": {
					"format": "int32",
					"in": "query",
					"maximum": 1000,
					"type": "integer"
				},
				"start": {
					"format": "int32",
					"in": "query",
					"maximum": 999,
					"type": "integer"
				}
			},
			"required": [
				"limit",
				"start"
			],
			"type": "object"
		}
	)"};

	std::string kHandlerProjectListJsonRequestParams{R"(
		{
			"start": "string",
			"limit": 5
		}
	)"};

	auto schemaDocument = formats::json::FromString(kHandlerProjectListJsonSchema);
	auto jsonDocument = formats::json::FromString(kHandlerProjectListJsonRequestParams);
	formats::json::Schema schema(schemaDocument);
	const auto validationResult = formats::json::Validate(jsonDocument, schema);
	LOG_WARNING() << "Validation Result = " << validationResult;

	//

	try {
		auto paging = parsePaging(req);
		res = _s.GetProjectList(paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
