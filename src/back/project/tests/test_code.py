import pytest


async def test_code(service_client):
	"""Code endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/code')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/code' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/code' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/code')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'repository_id': 123,
		'commit_hash': False
	}
	res = await service_client.post('/project/code', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'repository_id': '4572def0053fcba64e4becb1800a1d160502e99f',
		'commit_hash': False
	}
	res = await service_client.post('/project/code', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/code')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'xyz',
		'project_id': 'abc',
		'repository_id': 123,
		'commit_hash': False
	}
	res = await service_client.patch('/project/code', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'repository_id': '4572def0053fcba64e4becb1800a1d160502e99f',
		'commit_hash': False
	}
	res = await service_client.patch('/project/code', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/code')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/code' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/code' + '?id=1')
	assert res.status == 200