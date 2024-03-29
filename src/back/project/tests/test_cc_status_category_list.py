import pytest

endpoint = '/project/cc-status-category/list'

async def test_cc_status_category_list(service_client):
	"""Cc status category list endpoint"""

	"""Without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""With invalid params"""
	url = endpoint + '?start=-1&limit=abc&keepDeleted=x'
	res = await service_client.get(url)
	assert res.status == 400

	"""With valid params"""
	url = endpoint + '?start=0&limit=5&keepDeleted=true'
	res = await service_client.get(url)
	assert res.status == 200