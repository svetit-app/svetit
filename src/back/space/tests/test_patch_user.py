import pytest

url = '/space/user'
h = {'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}
json = {
	'spaceId': '11111111-1111-1111-1111-111111111111',
	'userId': '02d16a1d-18b1-4aaa-8b0f-f61915974c66',
	'isOwner': True,
	'roleId': 1, # hardcoded operator roleId (1)
	'joinedAt': 1
}


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_json_1_no_space_id(service_client):
	"""no spaceId param in request json"""
	data = json.copy()
	del data['spaceId']
	res = await service_client.patch(url, headers=h, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_json_2_no_user_id(service_client):
	"""no userId param in request json"""
	data = json.copy()
	del data['userId']
	res = await service_client.patch(url, headers=h, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_json_3_unknown_role(service_client):
	"""unknown role"""
	data = json.copy()
	h["X-User"] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['roleId'] = 99 # no exist roleId
	res = await service_client.patch(url, headers=h, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_1_self_patching(service_client):
	"""trying to patch myself"""
	data = json.copy()
	data['isOwner'] = False
	res = await service_client.patch(
		url, headers={'X-User': data['userId']}, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_2_caller_not_admin(service_client):
	"""caller is not an admin"""
	data = json.copy()
	data['isOwner'] = False
	data['userId'] = '01d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.patch(
		url, headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_3_caller_not_owner(service_client):
	"""caller is not an owner for transfering ownership"""
	data = json.copy()
	data['userId'] = '01d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.patch(
		url, headers={'X-User': '04d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json=data)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_4_target_is_an_owner(service_client):
	"""target user is an owner, can't change something for owner"""
	res = await service_client.patch(
		url, headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json=json)
	assert res.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_valid_ownership_transfer(service_client):
	"""valid ownership transfer"""
	data = json.copy()
	data['userId'] = '01d16a1d-18b1-4aaa-8b0f-f61915974c66'
	res = await service_client.patch(
		url, headers={'X-User': '02d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json=data)
	assert res.status == 204


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_valid_role_changing(service_client):
	"""valid role changing"""
	data = json.copy()
	data['userId'] = '03d16a1d-18b1-4aaa-8b0f-f61915974c66'
	data['roleId'] = 3 # hardcoded admin roleId (3)
	data['isOwner'] = False
	res = await service_client.patch(
		url, headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json=data)
	assert res.status == 204
