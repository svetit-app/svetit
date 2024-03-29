import pytest


async def test_value_view(service_client):
	"""Value view endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/value-view')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/value-view' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/value-view' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/value-view')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'di_type_id': 'abc',
		'value': 123,
		'view': 456,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/value-view', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'di_type_id': 1,
		'value': 'Test Value',
		'view': 'Test View',
		'is_deleted': False
	}
	res = await service_client.post('/project/value-view', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/value-view')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'xyz',
		'di_type_id': 'abc',
		'value': 123,
		'view': 456,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/value-view', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'di_type_id': 1,
		'value': 'Test Value',
		'view': 'Test View',
		'is_deleted': False
	}
	res = await service_client.patch('/project/value-view', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/value-view')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/value-view' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/value-view' + '?id=1')
	assert res.status == 200