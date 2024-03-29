import pytest

endpoint = '/project/di-type'

body_invalid = {
	'id': -1,
	'measure_id': 'abc',
	'save_timer_id': 'xyz',
	'key': 123,
	'name': 456,
	'mode': 'invalid',
	'save_algorithm': 'invalid',
	'is_deleted': 5
}

body_valid = {
	'id': 1,
	'measure_id': 1,
	'save_timer_id': 1,
	'key': 'abc123',
	'name': 'Test',
	'mode': 'readonly_flag',
	'save_algorithm': 'off',
	'is_deleted': False
}

async def test_di_type(service_client):
	"""Di type endpoint"""

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
	data = body_invalid.copy()
	del data['id']
	res = await service_client.post(endpoint, json=data)
	assert res.status == 400

	"""Post with valid body"""
	data = body_valid.copy()
	del data['id']
	res = await service_client.post(endpoint, json=data)
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