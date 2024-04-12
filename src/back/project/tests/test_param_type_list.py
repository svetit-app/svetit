import pytest

endpoint = '/project/param-type/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_param_type_list(service_client):
	"""Param type list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":6' in res.content