import pytest

endpoint = '/project/cc-type-di-type/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_type_di_type_list(service_client):
	"""Cc type di type list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content