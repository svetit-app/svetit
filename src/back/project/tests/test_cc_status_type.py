import pytest

endpoint = 'project/cc-status-type'

async def test_cc_status_type(service_client):
	"""Cc status type endpoint"""

	"""Get without param"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'category_id': 'xyz',
		'key': 123,
		'text': 456,
		'inform': 'test',
		'is_deleted': 'testing'
	}
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'category_id': 1,
		'key': 'abc123',
		'text': 'Test',
		'inform': False,
		'is_deleted': False
	}
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'def',
		'cc_type_id': 'abc',
		'category_id': 'xyz',
		'key': 123,
		'text': 456,
		'inform': 'test',
		'is_deleted': 'testing'
	}
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'cc_type_id': 1,
		'category_id': 1,
		'key':'abc123',
		'text': 'Test',
		'inform': False,
		'is_deleted': False
	}
	res = await service_client.patch(endpoint, json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid param"""
	url = endpoint + '?id=abc'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.delete(url)
	assert res.status == 200