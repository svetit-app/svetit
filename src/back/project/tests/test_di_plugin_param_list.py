import pytest

endpoint = '/project/di-plugin-param/list'

async def test_di_plugin_param_list(service_client):
	"""Di plugin param list endpoint"""

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