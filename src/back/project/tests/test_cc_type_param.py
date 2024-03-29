import pytest


async def test_cc_type_param(service_client):
	"""Cc type param endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/cc-type-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/cc-type-param' + '&ccTypeId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/cc-type-param' + '&ccTypeId=1&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-type-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/cc-type-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-type-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-type-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/cc-type-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-type-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/cc-type-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/cc-type-param' + '?ccTypeId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/cc-type-param' + '?ccTypeId=1&paramId=1')
	assert res.status == 200