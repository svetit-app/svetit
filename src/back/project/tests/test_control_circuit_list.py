import pytest

endpoint = '/project/control-circuit/list'


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_control_circuit_list(service_client):
	"""Control circuit list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5&sectionId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content
