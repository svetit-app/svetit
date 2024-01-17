import pytest


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_no_auth(service_client):
    response = await service_client.patch(
        '/space/user', json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 401


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_1_no_spaceId(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_2_no_userId(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_3_no_isOwner(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId":  "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "role": "guest", "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_4_no_role(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_5_spaceId_is_not_uuid(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "123abc", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_6_userId_is_not_string(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": 123, "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_7_isOwner_is_not_bool(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": "5", "role": "guest", "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_8_role_is_not_string(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": 123, "joinedAt": 1}
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_json_9_unknown_role(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
        json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "administrator", "joinedAt": 1}
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_unknown_caller(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 404


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_unknown_target_user(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '02d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "d2f9924d-a69a-4aec-b9c0-c80171d3ed86", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 404


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_logic_1_self_patching(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '02d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": False, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_logic_2_caller_is_not_an_admin(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": False, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_logic_3_caller_is_not_an_owner_when_transfer_ownership(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '04d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_invalid_logic_4_target_is_an_owner(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "02d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "guest", "joinedAt": 1}
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_valid_ownership_transfer(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '02d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": True, "role": "admin", "joinedAt": 1}
    )
    assert response.status == 200


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_patch_user_valid_role_changing(service_client):
    response = await service_client.patch(
        '/space/user', headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'}, json={"spaceId": "11111111-1111-1111-1111-111111111111", "userId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66", "isOwner": False, "role": "admin", "joinedAt": 1}
    )
    assert response.status == 200
