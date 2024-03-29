import pytest


async def test_cc_type_param_list(service_client):
	"""Cc type param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/cc-type-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/cc-type-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/cc-type-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200