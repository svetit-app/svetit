import pytest

endpoint = '/project/value-view/list'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_value_view_list(service_client):
	"""Value view list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&diTypeId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'View1' in res.content
	assert b'"total":2' in res.content
