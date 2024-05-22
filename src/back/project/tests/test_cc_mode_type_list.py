import pytest

endpoint = '/project/cc-mode-type/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_mode_type_list(service_client):
	"""Cc mode type list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&ccTypeId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content
