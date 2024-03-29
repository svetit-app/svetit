import pytest


async def test_di_type_list(service_client):
	"""Di type list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/di-type/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/di-type/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/di-type/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200
