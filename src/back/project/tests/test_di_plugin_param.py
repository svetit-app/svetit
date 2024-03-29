import pytest

endpoint = '/project/di-plugin-param'

async def test_di_plugin_param(service_client):
	"""Di plugin param endpoint"""

	"""Get without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid params"""
	url = endpoint + '?diTypeId=abc&paramId=xyz'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid params"""
	url = endpoint + '?diTypeId=1&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'di_type_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'di_type_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'di_type_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'di_type_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid params"""
	url = endpoint + '?diTypeId=abc&paramId=xyz'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid params"""
	url = endpoint + '?diTypeId=1&paramId=1'
	res = await service_client.delete(url)
	assert res.status == 200