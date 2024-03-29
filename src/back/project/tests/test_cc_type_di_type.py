import pytest


async def test_cc_type_di_type(service_client):
	"""Cc type di type endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/cc-type-di-type')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/cc-type-di-type' + '&ccTypeId=abc&diTypeId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/cc-type-di-type' + '&ccTypeId=1&diTypeId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-type-di-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'di_type_id': 'xyz',
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/cc-type-di-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'di_type_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-type-di-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-type-di-type')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'di_type_id': 'xyz',
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/cc-type-di-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'di_type_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-type-di-type', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/cc-type-di-type')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/cc-type-di-type' + '?ccTypeId=abc&diTypeId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/cc-type-di-type' + '?ccTypeId=1&diTypeId=1')
	assert res.status == 200