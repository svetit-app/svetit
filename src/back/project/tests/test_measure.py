import pytest


async def test_measure(service_client):
	"""Measure endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/measure')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/measure' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/measure' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/measure')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'name': 123,
		'is_deleted': -1
	}
	res = await service_client.post('/project/measure', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test measure',
		'is_deleted': False
	}
	res = await service_client.post('/project/measure', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/measure')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': -1,
		'project_id': 'abc',
		'name': 123,
		'is_deleted': -1
	}
	res = await service_client.patch('/project/measure', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test measure',
		'is_deleted': True
	}
	res = await service_client.patch('/project/measure', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/measure')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/measure' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/measure' + '?id=1')
	assert res.status == 200