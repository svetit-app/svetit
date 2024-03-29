import pytest


async def test_cc_mode_type(service_client):
	"""Cc mode type endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/cc-mode-type')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/cc-mode-type' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/cc-mode-type' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-mode-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'key': 123,
		'name': 456,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/cc-mode-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'key': 'abc123',
		'name': 'Test',
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-mode-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-mode-type')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'test',
		'cc_type_id': 'abc',
		'key': 123,
		'name': 456,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/cc-mode-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'cc_type_id': 1,
		'key': 'abc123',
		'name': 'Test',
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-mode-type', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/cc-mode-type')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/cc-mode-type' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/cc-mode-type' + '?id=1')
	assert res.status == 200