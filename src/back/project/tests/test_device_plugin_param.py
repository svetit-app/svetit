import pytest


async def test_device_plugin_param(service_client):
	"""Device plugin param endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/device-plugin-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/device-plugin-param' + '&deviceId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/device-plugin-param' + '&deviceId=1&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/device-plugin-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'device_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/device-plugin-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'device_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/device-plugin-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/device-plugin-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'device_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/device-plugin-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'device_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/device-plugin-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/device-plugin-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/device-plugin-param' + '?deviceId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/device-plugin-param' + '?deviceId=1&paramId=1')
	assert res.status == 200