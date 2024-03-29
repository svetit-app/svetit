import pytest


async def test_cc_status_category(service_client):
	"""Cc status category endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/cc-status-category')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/cc-status-category' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/cc-status-category' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-status-category')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 123,
		'key': 456,
		'name': 789,
		'color': 111,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/cc-status-category', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'key': 'abc123',
		'name': 'Test',
		'color': 'white',
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-status-category', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-status-category')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'abc',
		'project_id': 123,
		'key': 456,
		'name': 789,
		'color': 111,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/cc-status-category', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'key': 'abc123',
		'name': 'Test',
		'color': 'white',
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-status-category', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/cc-status-category')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/cc-status-category' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/cc-status-category' + '?id=1')
	assert res.status == 200