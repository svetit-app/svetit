import pytest


async def test_param_type_list(service_client):
	"""Param type list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/param-type/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/param-type/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/param-type/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200