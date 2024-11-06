import pytest

endpoint = '/project/cc-param/list'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_param_list(service_client):
	"""Cc param list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&ccId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content
