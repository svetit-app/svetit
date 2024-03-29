import pytest

endpoint = '/project/cc-type-di-type'

async def test_cc_type_di_type(service_client):
	"""Cc type di type endpoint"""

	"""Get without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid params"""
	url = endpoint + '?ccTypeId=abc&diTypeId=xyz'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid params"""
	url = endpoint + '?ccTypeId=1&diTypeId=1'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'di_type_id': 'xyz',
		'is_deleted': 'test'
	}
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'di_type_id': 1,
		'is_deleted': False
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'di_type_id': 'xyz',
		'is_deleted': 'test'
	}
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'di_type_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid params"""
	url = endpoint + '?ccTypeId=abc&diTypeId=xyz'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid params"""
	url = endpoint + '?ccTypeId=1&diTypeId=1'
	res = await service_client.delete(url)
	assert res.status == 200