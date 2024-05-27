#pragma once

#include "../model/project.hpp"
#include "../model/project_param.hpp"
#include "../model/section.hpp"
#include "../model/param_type.hpp"
#include "../model/section_param.hpp"
#include "../model/cc_type.hpp"
#include "../model/control_circuit.hpp"
#include "../model/plugin.hpp"
#include "../model/device.hpp"
#include "../model/device_plugin_param.hpp"
#include "../model/code.hpp"
#include "../model/measure.hpp"
#include "../model/save_timer.hpp"
#include "../model/cc_type_param.hpp"
#include "../model/di_type.hpp"
#include "../model/di_plugin_param.hpp"
#include "../model/cc_type_di_type.hpp"
#include "../model/device_item.hpp"
#include "../model/cc_mode_type.hpp"
#include "../model/cc_di.hpp"
#include "../model/cc_param.hpp"
#include "../model/cc_status_category.hpp"
#include "../model/cc_status_type.hpp"
#include "../model/value_view.hpp"
#include "../model/translation.hpp"
#include "../repo/repository.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <utility>

namespace svetit::project {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	model::ProjectParam GetProjectParam(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int paramId);
	void CreateProjectParam(const model::ProjectParam& projectParam);
	void DeleteProjectParam(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int paramId);
	PagingResult<model::ProjectParam> GetProjectParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit);

	model::SectionParam GetSectionParam(const boost::uuids::uuid& spaceId, int sectionId, int paramId);
	void CreateSectionParam(const model::SectionParam& sectionParam);
	void DeleteSectionParam(const boost::uuids::uuid& spaceId, int sectionId, int paramId);
	PagingResult<model::SectionParam> GetSectionParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit);

	model::DevicePluginParam GetDevicePluginParam(const boost::uuids::uuid& spaceId, int deviceId, int paramId);
	void CreateDevicePluginParam(const model::DevicePluginParam& devicePluginParam);
	void DeleteDevicePluginParam(const boost::uuids::uuid& spaceId, int deviceId, int paramId);
	PagingResult<model::DevicePluginParam> GetDevicePluginParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit);

	model::CcTypeParam GetCcTypeParam(const boost::uuids::uuid& spaceId, int ccTypeId, int paramId);
	void CreateCcTypeParam(const model::CcTypeParam& ccTypeParam);
	void DeleteCcTypeParam(const boost::uuids::uuid& spaceId, int ccTypeId, int paramId);
	PagingResult<model::CcTypeParam> GetCcTypeParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit);

	model::DiPluginParam GetDiPluginParam(const boost::uuids::uuid& spaceId, int diTypeId, int paramId);
	void CreateDiPluginParam(const model::DiPluginParam& diPluginParam);
	void DeleteDiPluginParam(const boost::uuids::uuid& spaceId, int diTypeId, int paramId);
	PagingResult<model::DiPluginParam> GetDiPluginParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit);

	const std::string& GetJSONSchemasPath();

	Repository& Repo();

private:
	Repository& _repo;
	int _itemsLimitForList;
	std::string _jsonSchemasPath;
};

} // namespace svetit::project
