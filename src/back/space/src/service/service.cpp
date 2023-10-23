#include "service.hpp"
#include "../model/model.hpp"

#include <chrono>
#include <limits>
#include <stdexcept>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/http/url.hpp"
#include <userver/crypto/base64.hpp>
#include <userver/utils/rand.hpp>

namespace svetit::space {

/*static*/ yaml_config::Schema Service::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main service component
additionalProperties: false
properties:
 someUrl:
    type: string
    description: Path to some page
    defaultDescription: ''
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _someUrl{""}
{
	_spaces.push_back({"11111111-1111-1111-1111-111111111111", "Пространство №1", "key1", true, std::chrono::system_clock::now()});
	_spaces.push_back({"22222222-2222-2222-2222-222222222222", "Пространство №2", "key2", true, std::chrono::system_clock::now()});
	_spaces.push_back({"33333333-3333-3333-3333-333333333333", "Пространство №3", "key3", true, std::chrono::system_clock::now()});
	_spaces.push_back({"44444444-4444-4444-4444-444444444444", "Пространство №4", "key4", true, std::chrono::system_clock::now()});
	_spaces.push_back({"55555555-5555-5555-5555-555555555555", "Пространство №5", "key5", true, std::chrono::system_clock::now()});
	_spaces.push_back({"66666666-6666-6666-6666-666666666666", "Пространство №6", "key6", true, std::chrono::system_clock::now()});
	_spaces.push_back({"77777777-7777-7777-7777-777777777777", "Пространство №7", "key7", true, std::chrono::system_clock::now()});
	_spaces.push_back({"88888888-8888-8888-8888-888888888888", "Пространство №8", "key8", true, std::chrono::system_clock::now()});
	_spaces.push_back({"99999999-9999-9999-9999-999999999999", "Пространство №9", "key9", true, std::chrono::system_clock::now()});
	_spaces.push_back({"10000000-1000-1000-1000-100000000000", "Пространство №10", "key10", true, std::chrono::system_clock::now()});
	_spaces.push_back({"11000000-1100-1100-1100-110000000000", "Пространство №11", "key11", true, std::chrono::system_clock::now()});
}

std::vector<svetit::space::Space> Service::GetList()
{
	return _spaces;
}

} // namespace svetit::space