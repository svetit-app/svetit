import pytest


async def test_project_param_list(service_client):
	"""Project param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/project-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/project-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/project-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200