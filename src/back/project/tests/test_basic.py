import pytest

h = {}
json = {}

# run by make test-project or make run-project
async def test_details(service_client):
	"""Discovery request to details endpoint"""
	res = await service_client.post('/details')
	assert res.status == 200


async def test_project(service_client):
	"""Discovery requests to project endpoint"""
	"""Get by id (uuid)"""
	res = await service_client.get('/project')
	assert res.status == 200

	"""Get by key"""
	res = await service_client.get('/project')
	assert res.status == 200

	res = await service_client.post('/project')
	assert res.status == 200

	res = await service_client.patch('/project')
	assert res.status == 200

	res = await service_client.delete('/project')
	assert res.status == 200


async def test_project_list(service_client):
	"""Discovery request to project list endpoint"""
	res = await service_client.get('/project/list')
	assert res.status == 200


async def test_param_type(service_client):
	"""Discovery requests to param_type endpoint"""
	res = await service_client.get('/param_type')
	assert res.status == 200

	res = await service_client.post('/param_type')
	assert res.status == 200

	res = await service_client.patch('/param_type')
	assert res.status == 200

	res = await service_client.delete('/param_type')
	assert res.status == 200


async def test_param_type_list(service_client):
	"""Discovery request to param_type list endpoint"""
	res = await service_client.get('/param_type/list')
	assert res.status == 200


async def test_project_param(service_client):
	"""Discovery requests to project_param endpoint"""
	res = await service_client.get('/project_param')
	assert res.status == 200

	res = await service_client.post('/project_param')
	assert res.status == 200

	res = await service_client.patch('/project_param')
	assert res.status == 200

	res = await service_client.delete('/project_param')
	assert res.status == 200


async def test_project_param_list(service_client):
	"""Discovery request to project_param list endpoint"""
	res = await service_client.get('/project_param/list')
	assert res.status == 200


async def test_section(service_client):
	"""Discovery requests to section endpoint"""
	res = await service_client.get('/section')
	assert res.status == 200

	res = await service_client.post('/section')
	assert res.status == 200

	res = await service_client.patch('/section')
	assert res.status == 200

	res = await service_client.delete('/section')
	assert res.status == 200


async def test_section_list(service_client):
	"""Discovery request to section list endpoint"""
	res = await service_client.get('/section/list')
	assert res.status == 200


async def test_section_param(service_client):
	"""Discovery requests to section_param endpoint"""
	res = await service_client.get('/section_param')
	assert res.status == 200

	res = await service_client.post('/section_param')
	assert res.status == 200

	res = await service_client.patch('/section_param')
	assert res.status == 200

	res = await service_client.delete('/section_param')
	assert res.status == 200


async def test_section_param_list(service_client):
	"""Discovery request to section_param list endpoint"""
	res = await service_client.get('/section_param/list')
	assert res.status == 200


async def test_cc_type(service_client):
	"""Discovery requests to cc_type endpoint"""
	res = await service_client.get('/cc_type')
	assert res.status == 200

	res = await service_client.post('/cc_type')
	assert res.status == 200

	res = await service_client.patch('/cc_type')
	assert res.status == 200

	res = await service_client.delete('/cc_type')
	assert res.status == 200


async def test_cc_type_list(service_client):
	"""Discovery request to cc_type list endpoint"""
	res = await service_client.get('/cc_type/list')
	assert res.status == 200


async def test_control_circuit(service_client):
	"""Discovery requests to control_circuit endpoint"""
	res = await service_client.get('/control_circuit')
	assert res.status == 200

	res = await service_client.post('/control_circuit')
	assert res.status == 200

	res = await service_client.patch('/control_circuit')
	assert res.status == 200

	res = await service_client.delete('/control_circuit')
	assert res.status == 200


async def test_control_circuit_list(service_client):
	"""Discovery request to control_circuit list endpoint"""
	res = await service_client.get('/control_circuit/list')
	assert res.status == 200


async def test_plugin(service_client):
	"""Discovery requests to plugin endpoint"""
	res = await service_client.get('/plugin')
	assert res.status == 200

	res = await service_client.post('/plugin')
	assert res.status == 200

	res = await service_client.patch('/plugin')
	assert res.status == 200

	res = await service_client.delete('/plugin')
	assert res.status == 200


async def test_plugin_list(service_client):
	"""Discovery request to plugin list endpoint"""
	res = await service_client.get('/plugin/list')
	assert res.status == 200


async def test_device(service_client):
	"""Discovery requests to device endpoint"""
	res = await service_client.get('/device')
	assert res.status == 200

	res = await service_client.post('/device')
	assert res.status == 200

	res = await service_client.patch('/device')
	assert res.status == 200

	res = await service_client.delete('/device')
	assert res.status == 200


async def test_device_list(service_client):
	"""Discovery request to device list endpoint"""
	res = await service_client.get('/device/list')
	assert res.status == 200


async def test_device_plugin_param(service_client):
	"""Discovery requests to device_plugin_param endpoint"""
	res = await service_client.get('/device_plugin_param')
	assert res.status == 200

	res = await service_client.post('/device_plugin_param')
	assert res.status == 200

	res = await service_client.patch('/device_plugin_param')
	assert res.status == 200

	res = await service_client.delete('/device_plugin_param')
	assert res.status == 200


async def test_device_plugin_param_list(service_client):
	"""Discovery request to device_plugin_param list endpoint"""
	res = await service_client.get('/device_plugin_param/list')
	assert res.status == 200


async def test_code(service_client):
	"""Discovery requests to code endpoint"""
	res = await service_client.get('/code')
	assert res.status == 200

	res = await service_client.post('/code')
	assert res.status == 200

	res = await service_client.patch('/code')
	assert res.status == 200

	res = await service_client.delete('/code')
	assert res.status == 200


async def test_code_list(service_client):
	"""Discovery request to code list endpoint"""
	res = await service_client.get('/code/list')
	assert res.status == 200


async def test_measure(service_client):
	"""Discovery requests to measure endpoint"""
	res = await service_client.get('/measure')
	assert res.status == 200

	res = await service_client.post('/measure')
	assert res.status == 200

	res = await service_client.patch('/measure')
	assert res.status == 200

	res = await service_client.delete('/measure')
	assert res.status == 200


async def test_measure_list(service_client):
	"""Discovery request to measure list endpoint"""
	res = await service_client.get('/measure/list')
	assert res.status == 200


async def test_save_timer(service_client):
	"""Discovery requests to save_timer endpoint"""
	res = await service_client.get('/save_timer')
	assert res.status == 200

	res = await service_client.post('/save_timer')
	assert res.status == 200

	res = await service_client.patch('/save_timer')
	assert res.status == 200

	res = await service_client.delete('/save_timer')
	assert res.status == 200


async def test_save_timer_list(service_client):
	"""Discovery request to save_timer list endpoint"""
	res = await service_client.get('/save_timer/list')
	assert res.status == 200


async def test_cc_type_param(service_client):
	"""Discovery requests to cc_type_param endpoint"""
	res = await service_client.get('/cc_type_param')
	assert res.status == 200

	res = await service_client.post('/cc_type_param')
	assert res.status == 200

	res = await service_client.patch('/cc_type_param')
	assert res.status == 200

	res = await service_client.delete('/cc_type_param')
	assert res.status == 200


async def test_cc_type_param_list(service_client):
	"""Discovery request to cc_type_param list endpoint"""
	res = await service_client.get('/cc_type_param/list')
	assert res.status == 200


async def test_di_type(service_client):
	"""Discovery requests to di_type endpoint"""
	res = await service_client.get('/di_type')
	assert res.status == 200

	res = await service_client.post('/di_type')
	assert res.status == 200

	res = await service_client.patch('/di_type')
	assert res.status == 200

	res = await service_client.delete('/di_type')
	assert res.status == 200


async def test_di_type_list(service_client):
	"""Discovery request to di_type list endpoint"""
	res = await service_client.get('/di_type/list')
	assert res.status == 200


async def test_di_plugin_param(service_client):
	"""Discovery requests to di_plugin_param endpoint"""
	res = await service_client.get('/di_plugin_param')
	assert res.status == 200

	res = await service_client.post('/di_plugin_param')
	assert res.status == 200

	res = await service_client.patch('/di_plugin_param')
	assert res.status == 200

	res = await service_client.delete('/di_plugin_param')
	assert res.status == 200


async def test_di_plugin_param_list(service_client):
	"""Discovery request to di_plugin_param list endpoint"""
	res = await service_client.get('/di_plugin_param/list')
	assert res.status == 200


async def test_cc_type_di_type(service_client):
	"""Discovery requests to cc_type_di_type endpoint"""
	res = await service_client.get('/cc_type_di_type')
	assert res.status == 200

	res = await service_client.post('/cc_type_di_type')
	assert res.status == 200

	res = await service_client.patch('/cc_type_di_type')
	assert res.status == 200

	res = await service_client.delete('/cc_type_di_type')
	assert res.status == 200


async def test_cc_type_di_type_list(service_client):
	"""Discovery request to cc_type_di_type list endpoint"""
	res = await service_client.get('/cc_type_di_type/list')
	assert res.status == 200


async def test_device_item(service_client):
	"""Discovery requests to device_item endpoint"""
	res = await service_client.get('/device_item')
	assert res.status == 200

	res = await service_client.post('/device_item')
	assert res.status == 200

	res = await service_client.patch('/device_item')
	assert res.status == 200

	res = await service_client.delete('/device_item')
	assert res.status == 200


async def test_device_item_list(service_client):
	"""Discovery request to device_item list endpoint"""
	res = await service_client.get('/device_item/list')
	assert res.status == 200


async def test_cc_mode_type(service_client):
	"""Discovery requests to cc_mode_type endpoint"""
	res = await service_client.get('/cc_mode_type')
	assert res.status == 200

	res = await service_client.post('/cc_mode_type')
	assert res.status == 200

	res = await service_client.patch('/cc_mode_type')
	assert res.status == 200

	res = await service_client.delete('/cc_mode_type')
	assert res.status == 200


async def test_cc_mode_type_list(service_client):
	"""Discovery request to cc_mode_type list endpoint"""
	res = await service_client.get('/cc_mode_type/list')
	assert res.status == 200


async def test_cc_di(service_client):
	"""Discovery requests to cc_di endpoint"""
	res = await service_client.get('/cc_di')
	assert res.status == 200

	res = await service_client.post('/cc_di')
	assert res.status == 200

	res = await service_client.patch('/cc_di')
	assert res.status == 200

	res = await service_client.delete('/cc_di')
	assert res.status == 200


async def test_cc_di_list(service_client):
	"""Discovery request to cc_di list endpoint"""
	res = await service_client.get('/cc_di/list')
	assert res.status == 200


async def test_cc_param(service_client):
	"""Discovery requests to cc_param endpoint"""
	res = await service_client.get('/cc_param')
	assert res.status == 200

	res = await service_client.post('/cc_param')
	assert res.status == 200

	res = await service_client.patch('/cc_param')
	assert res.status == 200

	res = await service_client.delete('/cc_param')
	assert res.status == 200


async def test_cc_param_list(service_client):
	"""Discovery request to cc_param list endpoint"""
	res = await service_client.get('/cc_param/list')
	assert res.status == 200


async def test_cc_status_category(service_client):
	"""Discovery requests to cc_status_category endpoint"""
	res = await service_client.get('/cc_status_category')
	assert res.status == 200

	res = await service_client.post('/cc_status_category')
	assert res.status == 200

	res = await service_client.patch('/cc_status_category')
	assert res.status == 200

	res = await service_client.delete('/cc_status_category')
	assert res.status == 200


async def test_cc_status_category_list(service_client):
	"""Discovery request to cc_status_category list endpoint"""
	res = await service_client.get('/cc_status_category/list')
	assert res.status == 200


async def test_cc_status_type(service_client):
	"""Discovery requests to cc_status_type endpoint"""
	res = await service_client.get('/cc_status_type')
	assert res.status == 200

	res = await service_client.post('/cc_status_type')
	assert res.status == 200

	res = await service_client.patch('/cc_status_type')
	assert res.status == 200

	res = await service_client.delete('/cc_status_type')
	assert res.status == 200


async def test_cc_status_type_list(service_client):
	"""Discovery request to cc_status_type list endpoint"""
	res = await service_client.get('/cc_status_type/list')
	assert res.status == 200


async def test_value_view(service_client):
	"""Discovery requests to value_view endpoint"""
	res = await service_client.get('/value_view')
	assert res.status == 200

	res = await service_client.post('/value_view')
	assert res.status == 200

	res = await service_client.patch('/value_view')
	assert res.status == 200

	res = await service_client.delete('/value_view')
	assert res.status == 200


async def test_value_view_list(service_client):
	"""Discovery request to value_view list endpoint"""
	res = await service_client.get('/value_view/list')
	assert res.status == 200


async def test_translation(service_client):
	"""Discovery requests to translation endpoint"""
	res = await service_client.get('/translation')
	assert res.status == 200

	res = await service_client.post('/translation')
	assert res.status == 200

	res = await service_client.patch('/translation')
	assert res.status == 200

	res = await service_client.delete('/translation')
	assert res.status == 200


async def test_translation_list(service_client):
	"""Discovery request to translation list endpoint"""
	res = await service_client.get('/translation/list')
	assert res.status == 200