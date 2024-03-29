import pytest


async def test_cc_di(service_client):
	"""Cc di endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/cc-di')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/cc-di' + '&ccId=abc&diId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/cc-di' + '&ccId=1&diId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-di')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_id': 'abc',
		'di_id': 'xyz',
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/cc-di', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_id': 1,
		'di_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-di', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-di')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'cc_id': 'abc',
		'di_id': 'xyz',
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/cc-di', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'cc_id': 1,
		'di_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-di', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/cc-di')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/cc-di' + '?ccId=abc&diId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/cc-di' + '?ccId=1&diId=1')
	assert res.status == 200