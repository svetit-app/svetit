import pytest


async def test_project(service_client):
	"""Project endpoint"""
	"""Without params"""
	res = await service_client.get('/project')
	assert res.status == 400

	"""By id with invalid uuid"""
	res = await service_client.get('/project' + '?id=123')
	assert res.status == 400

	"""By id with valid uuid"""
	res = await service_client.get('/project' + '?id=11111111-1111-1111-1111-111111111111')
	assert res.status == 200

	"""By key with empty key"""
	res = await service_client.get('/project' + '?key=')
	assert res.status == 400

	"""By key with valid key"""
	res = await service_client.get('/project' + '?key=project123')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'space_id': 'abc123',
		'key': 123,
		'name': True,
		'description': '',
		'sync': 'invalid_sync'
	}
	res = await service_client.post('/project', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'space_id': '33333333-3333-3333-3333-333333333333',
		'key': 'test',
		'name': 'Test project',
		'description': 'Text',
		'sync': 'project_to_node'
	}
	res = await service_client.post('/project', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'abc',
		'space_id': 'abc123',
		'key': 123,
		'name': True,
		'description': '',
		'sync': 'invalid_sync'
	}
	res = await service_client.patch('/project', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': '11111111-1111-1111-1111-111111111111',
		'space_id': '33333333-3333-3333-3333-333333333333',
		'key': 'test',
		'name': 'Test project',
		'description': 'Text',
		'sync': 'project_to_node'
	}
	res = await service_client.post('/project', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project')
	assert res.status == 400

	"""Delete with invalid param (invalid uuid)"""
	res = await service_client.delete('/project' + '?id=123')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project' + '?id=11111111-1111-1111-1111-111111111111')
	assert res.status == 200