import pytest


async def test_di_type(service_client):
	"""Di type endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/di-type')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/di-type' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/di-type' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/di-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'measure_id': 'abc',
		'save_timer_id': 'xyz',
		'key': 123,
		'name': 456,
		'mode': 'invalid',
		'save_algorithm': 'invalid',
		'is_deleted': 5
	}
	res = await service_client.post('/project/di-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'measure_id': 1,
		'save_timer_id': 1,
		'key': 'abc123',
		'name': 'Test',
		'mode': 'readonly_flag',
		'save_algorithm': 'off',
		'is_deleted': False
	}
	res = await service_client.post('/project/di-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/di-type')
	assert res.status == 400

	"""Patch with invalid body"""
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
	res = await service_client.patch('/project/di-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
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
	res = await service_client.patch('/project/di-type', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/di-type')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/di-type' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/di-type' + '?id=1')
	assert res.status == 200