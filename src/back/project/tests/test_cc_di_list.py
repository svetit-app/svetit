import pytest

endpoint = '/project/cc-di/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_di_list(service_client):
	"""Cc di list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&ccId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":1' in res.content
