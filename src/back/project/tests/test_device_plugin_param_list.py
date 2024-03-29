import pytest


async def test_device_plugin_param_list(service_client):
	"""Device plugin param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/device-plugin-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/device-plugin-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/device-plugin-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200