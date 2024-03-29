import pytest


async def test_cc_type(service_client):
	"""Cc type endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/cc-type')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/cc-type' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/cc-type' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'key': 123,
		'name': -1,
		'description': 1,
		'is_deleted': 'abc'
	}
	res = await service_client.post('/project/cc-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'key': 'abc123',
		'name': 'Test cc_type',
		'description': 'Description',
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-type')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': '',
		'project_id': 'abc',
		'key': 123,
		'name': -1,
		'description': 1,
		'is_deleted': 'abc'
	}
	res = await service_client.patch('/project/cc-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'key': 'abc123',
		'name': 'Test cc_type',
		'description': 'Description',
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-type', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/cc-type')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/cc-type' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/cc-type' + '?id=1')
	assert res.status == 200