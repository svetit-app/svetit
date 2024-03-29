import pytest


async def test_device(service_client):
	"""Device endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/device')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/device' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/device' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/device')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'klm',
		'plugin_id': 'def',
		'name': 123,
		'check_interval_msec': 'xyz',
		'is_deleted': 'abc'
	}
	res = await service_client.post('/project/device', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'plugin_id': 1,
		'name': 'Test device',
		'check_interval_msec': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/device', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/device')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'zzz',
		'project_id': 'klm',
		'plugin_id': 'def',
		'name': 123,
		'check_interval_msec': 'xyz',
		'is_deleted': 'abc'
	}
	res = await service_client.patch('/project/device', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'plugin_id': 1,
		'name': 'Test device',
		'check_interval_msec': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/device', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/device')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/device' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/device' + '?id=1')
	assert res.status == 200