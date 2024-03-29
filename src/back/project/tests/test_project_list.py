import pytest

endpoint = '/project/list'

async def test_project_list(service_client):
	"""Project list endpoint"""
	"""Without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""With invalid params"""
	url = endpoint + '?start=abc&limit=-1'
	res = await service_client.get(url)
	assert res.status == 400

	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200