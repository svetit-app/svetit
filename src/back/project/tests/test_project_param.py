import pytest


async def test_project_param(service_client):
	"""Project param endpoint"""
	"""Get without params"""
	res = await service_client.get('/project/project-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/project-param' + '&projectId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/project-param' + '&projectId=11111111-1111-1111-1111-111111111111&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/project-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/project-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/project-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/project-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/project-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'param_id': 1,
		'is_deleted': True
	}
	res = await service_client.patch('/project/project-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/project-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/project-param' + '?projectId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/project-param' + '?projectId=11111111-1111-1111-1111-111111111111&paramId=1')
	assert res.status == 200