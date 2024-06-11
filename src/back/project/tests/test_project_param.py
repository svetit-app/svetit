import pytest

endpoint = '/project/project-param'

body = {
	'projectId': '11111111-1111-1111-1111-111111111111',
	'paramId': 3
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_project_param(service_client):
	"""Project param endpoint"""
	"""Get with valid params"""
	url = endpoint + '?projectId=11111111-1111-1111-1111-111111111111&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'projectId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Delete with valid params"""
	url = endpoint + '?projectId=11111111-1111-1111-1111-111111111111&paramId=1'
	res = await service_client.delete(url)
	assert res.status == 204