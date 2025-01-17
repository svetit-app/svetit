import pytest

endpoint = '/project/cc-type-param/list'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_type_param_list(service_client):
	"""Cc type param list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&ccTypeId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content
