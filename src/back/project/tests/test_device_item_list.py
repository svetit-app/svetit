import pytest

endpoint = '/project/device-item/list'

async def test_device_item_list(service_client):
	"""Device item list endpoint"""

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