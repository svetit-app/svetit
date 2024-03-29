import pytest


async def test_cc_di_list(service_client):
	"""Cc di list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/cc-di/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/cc-di/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/cc-di/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200