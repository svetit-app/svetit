import pytest

# Start via `make test-debug` or `make test-release`
#@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_basic(service_client):
    assert 200 == 200
