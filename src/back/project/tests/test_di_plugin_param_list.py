import pytest

endpoint = '/project/di-plugin-param/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_di_plugin_param_list(service_client):
	"""Di plugin param list endpoint"""

	"""Without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""With invalid params"""
	url = endpoint + '?start=-1&limit=abc&keepDeleted=x'
	res = await service_client.get(url)
	assert res.status == 400

	"""With valid params (keepDeleted = True)"""
	url = endpoint + '?start=0&limit=5&keepDeleted=true'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content

	"""With valid params (keepDeleted = False)"""
	url = endpoint + '?start=0&limit=5&keepDeleted=false'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":1' in res.content