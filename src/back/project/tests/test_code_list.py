import pytest


async def test_code_list(service_client):
	"""Code list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/code/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/code/list' + '?start=-1&limit=abc')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/code/list' + '?start=0&limit=5')
	assert res.status == 200