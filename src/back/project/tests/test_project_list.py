import pytest


async def test_project_list(service_client):
	"""Project list endpoint"""
	"""Without params"""
	res = await service_client.get('/project/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/list' + '?start=abc&limit=-1')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/list' + '?start=0&limit=5')
	assert res.status == 200