import pytest


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_json_1_no_space_id(service_client):
    """no spaceId param in request json"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={
            "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": True,
            "role": "guest",
            "joinedAt": 1
        }
    )
    assert response.status == 500


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_json_2_no_user_id(service_client):
    """no userId param in request json"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "isOwner": True,
            "role": "guest",
            "joinedAt": 1
        }
    )
    assert response.status == 500


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_json_3_unknown_role(service_client):
    """unknown role"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": True,
            "role": "administrator",
            "joinedAt": 1
        }
    )
    assert response.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_1_self_patching(service_client):
    """trying to patch myself"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': '02d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": False,
            "role": "guest",
            "joinedAt": 1
        }
    )
    assert response.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_2_caller_not_admin(service_client):
    """caller is not an admin"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": False,
            "role": "guest",
            "joinedAt": 1
        }
    )
    assert response.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_3_caller_not_owner(service_client):
    """caller is not an owner for transfering ownership"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': '04d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": True,
            "role": "guest",
            "joinedAt": 1
        }
    )
    assert response.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_invalid_logic_4_target_is_an_owner(service_client):
    """target user is an owner, can't change something for owner"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": True,
            "role": "guest",
            "joinedAt": 1
        }
    )
    assert response.status == 400


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_valid_ownership_transfer(service_client):
    """valid ownership transfer"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': '02d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": True,
            "role": "admin",
            "joinedAt": 1
        }
    )
    assert response.status == 200


@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_patch_user_valid_role_changing(service_client):
    """valid role changing"""
    response = await service_client.patch(
        '/space/user',
        headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "isOwner": False,
            "role": "admin",
            "joinedAt": 1
        }
    )
    assert response.status == 200
