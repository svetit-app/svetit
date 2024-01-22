import pytest


# run by make test-space or make run-space
@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_post_invitation_no_auth(service_client):
    """No authorization X-User header"""
    response = await service_client.post(
        '/space/invitation',
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 401


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_post_invitation_wrong_params(service_client):
    """Wrong params"""
    # no spaceId
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 500

    # no userId
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "role": "user",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 500

    # no role
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 500

    # no creatorId
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "createdAt": 0
        }
    )
    assert response.status == 500


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_post_invitation_invalid_params(service_client):
    """Wrong params"""
    # empty spaceId
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "",
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400

    # spaceId is not ad valid uuid
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "123",
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 500

    # wrong role
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "87d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "administrator",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400

    # empty userId
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "",
            "role": "user",
            "creatorId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400

    # empty userId
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "12d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "",
            "createdAt": 0
        }
    )
    assert response.status == 400


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_post_invitation_valid(service_client):
    """Valid login of invitation creation"""
    # I want to join to space where requestAllowed=true
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "20d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "20d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 201

    # We want to join some user to space where X-User is admin
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '01d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "88d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "01d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 201


@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_post_invitation_invalid(service_client):
    """Valid login of invitation creation"""
    # Unknown space
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '8ad16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "00000000-1111-1111-1111-111111111111",
            "userId": "20d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "20d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400

    # user is already in space, can't join myself
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400

    # creator=user, creator is not in space, but requsetSallowed=false
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "12000000-1200-1200-1200-120000000000",
            "userId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400

    # creator<>user, creator is in space, but not an admin
    response = await service_client.post(
        '/space/invitation',
        headers={'X-User': '03d16a1d-18b1-4aaa-8b0f-f61915974c66'},
        json={
            "id": 0,
            "spaceId": "11111111-1111-1111-1111-111111111111",
            "userId": "14d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "role": "user",
            "creatorId": "03d16a1d-18b1-4aaa-8b0f-f61915974c66",
            "createdAt": 0
        }
    )
    assert response.status == 400
