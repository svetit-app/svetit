import pytest


async def test_plugin(service_client):
	"""Plugin endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/plugin')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/plugin' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/plugin' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/plugin')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'name': 123,
		'description': False,
		'key': 0,
		'is_deleted': 5
	}
	res = await service_client.post('/project/plugin', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test plugin',
		'description': 'Description',
		'key': 'abc123',
		'is_deleted': False
	}
	res = await service_client.post('/project/plugin', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/plugin')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'xyz',
		'project_id': 'abc',
		'name': 123,
		'description': False,
		'key': 0,
		'is_deleted': 5
	}
	res = await service_client.patch('/project/plugin', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test plugin',
		'description': 'Description',
		'key': 'abc123',
		'is_deleted': False
	}
	res = await service_client.patch('/project/plugin', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/plugin')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/plugin' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/plugin' + '?id=1')
	assert res.status == 200