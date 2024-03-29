import pytest


async def test_device_item(service_client):
	"""Device item endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/device-item')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/device-item' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/device-item' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/device-item')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'device_id': 'abc',
		'type_id': 'xyz',
		'name': 123,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/device-item', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'device_id': 1,
		'type_id': 1,
		'name': 'Test',
		'is_deleted': False
	}
	res = await service_client.post('/project/device-item', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/device-item')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'invalid',
		'device_id': 'abc',
		'type_id': 'xyz',
		'name': 123,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/device-item', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'device_id': 1,
		'type_id': 1,
		'name': 'Test',
		'is_deleted': False
	}
	res = await service_client.patch('/project/device-item', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/device-item')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/device-item' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/device-item' + '?id=1')
	assert res.status == 200