import pytest


async def test_measure_list(service_client):
	"""Measure list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/measure/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/measure/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/measure/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200