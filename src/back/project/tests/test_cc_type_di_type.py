import pytest

endpoint = '/project/cc-type-di-type'

body_invalid = {
	'cc_type_id': 'abc',
	'di_type_id': 'xyz',
	'is_deleted': 'test'
}

body_valid = {
	'cc_type_id': 1,
	'di_type_id': 2,
	'is_deleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
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
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""

	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['is_deleted'] = True
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid params"""
	url = endpoint + '?ccTypeId=abc&diTypeId=xyz'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid params"""
	url = endpoint + '?ccTypeId=1&diTypeId=2'
	res = await service_client.delete(url)
	assert res.status == 200