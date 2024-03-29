import pytest

endpoint = '/project/device-plugin-param'

body_invalid = {
	'device_id': 'abc',
	'param_id': -1,
	'is_deleted': 'test'
}

body_valid = {
	'device_id': 1,
	'param_id': 1,
	'is_deleted': False
}

async def test_device_plugin_param(service_client):
	"""Device plugin param endpoint"""

	"""Get without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid params"""
	url = endpoint + '?deviceId=abc&paramId=xyz'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid params"""
	url = endpoint + '?deviceId=1&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	res = await service_client.patch(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid params"""
	url = endpoint + '?deviceId=abc&paramId=xyz'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid params"""
	url = endpoint + '?deviceId=1&paramId=1'
	res = await service_client.delete(url)
	assert res.status == 200