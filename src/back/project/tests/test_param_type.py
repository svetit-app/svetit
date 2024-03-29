import pytest

endpoint = '/project/param-type'

async def test_param_type(service_client):
	"""Param type endpoint"""

	"""Get without param"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'parent_id': 'abc',
		'key': 123,
		'name': True,
		'description': -1,
		'value_type': 'unknown',
		'is_deleted': 123
	}
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'parent_id': 1,
		'key': 'abc123',
		'name': 'Test name',
		'description': 'Test description',
		'value_type': 'int',
		'is_deleted': False
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': -1,
		'parent_id': 'abc',
		'key': 123,
		'name': True,
		'description': -1,
		'value_type': 'unknown',
		'is_deleted': 123
	}
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 2,
		'parent_id': 1,
		'key': 'abc123',
		'name': 'Test name',
		'description': 'Test description',
		'value_type': 'int',
		'is_deleted': False
	}
	res = await service_client.patch(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.delete(url)
	assert res.status == 200