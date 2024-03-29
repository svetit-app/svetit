import pytest

endpoint = '/project/control-circuit'

async def test_control_circuit(service_client):
	"""Control circuit endpoint"""

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
		'type_id': 'abc',
		'section_id': 'xyz',
		'name': -1,
		'is_deleted': 123
	}
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'type_id': 1,
		'section_id': 1,
		'name': 'Test control curcuit',
		'is_deleted': False
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'def',
		'type_id': 'abc',
		'section_id': 'xyz',
		'name': -1,
		'is_deleted': 123
	}
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'type_id': 1,
		'section_id': 1,
		'name': 'Test control curcuit',
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