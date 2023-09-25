This instruction [source](https://www.keycloak.org/getting-started/getting-started-docker) in keycloak web site.

# Start
1. start keycloak docker-compose up
2. see .env file for admin credentials

# Log in to admin console
1. open [admin console](http://localhost:8081/admin)
2. use admin credentials for log in

# Create realm
1. You need to click to **Create realm** in the top-left corner
2. Enter '**svetit**' in Realm name

# Create test user
1. Click 'Users' in the left-hand menu.
2. Click 'Create new user'
3. Fill 'Username', 'First' and 'Last name'.
4. Click 'Create'
5. Go to 'Credentials' tab.
6. Set new password.

# Create client
1. Click 'Clients' in the left-hand menu.
2. Click 'Create client'.
3. Set client type to 'OpenID Connect'
4. Set client ID to 'web'
5. Click Next
6. Confirm that 'Standard flow' is enabled.
7. Check 'Client authentication'
8. Click Save.
9. Add to 'Valid redirect URIs':
  1. https://svetit.io/api/*
  2. http://localhost:8080/api/*
10. Add to 'Web origins': // Maybe just add '+' it's enough.
  1. https://svetit.io
  2. http://localhost:8080
11. Set 'Front channel logout' to false
