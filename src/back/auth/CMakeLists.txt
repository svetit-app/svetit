cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(svetit_auth CXX)

set(SERVICE_PORT 8082 CACHE INT "Service port for listening")

# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
	# Enable userver libraries that are needed in this project
	set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

	# Compatibility mode: some systems don't support these features
	set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
	set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
	set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

	if (EXISTS third_party/userver)
		message(STATUS "Using userver framework from third_party/userver")
		add_subdirectory(third_party/userver)
	else()
		message(FATAL_ERROR "Either install the userver or provide a path to it")
	endif()
endif()

userver_setup_environment()

find_package(svetit REQUIRED)

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
	src/model/model.hpp
	src/model/session.hpp
	src/model/oidctokens.hpp
	src/model/oidctokens_serialize.hpp
	src/model/oidctokens_serialize.cpp
	src/model/userinfo.hpp
	src/model/userinfo_serialize.hpp
	src/model/userinfo_serialize.cpp
	src/model/session_refresh.hpp
	src/model/session_refresh_serialize.hpp
	src/model/session_refresh_serialize.cpp

	src/api/helpers.hpp
	src/api/helpers.cpp
	src/api/token_introspect.hpp
	src/api/token_introspect.cpp
	src/api/token_refresh.hpp
	src/api/token_refresh.cpp
	src/api/login.hpp
	src/api/login.cpp
	src/api/login_callback.hpp
	src/api/login_callback.cpp
	src/api/logout.hpp
	src/api/logout.cpp
	src/api/logout_callback.hpp
	src/api/logout_callback.cpp
	src/api/user_info.hpp
	src/api/user_info.cpp
	src/api/user_byid.hpp
	src/api/user_byid.cpp
	src/api/user_list.hpp
	src/api/user_list.cpp

	src/repo/repository.hpp
	src/repo/repository.cpp
	src/repo/table_state.hpp
	src/repo/table_state.cpp
	src/repo/table_session.hpp
	src/repo/table_session.cpp

	src/service/tokenizer.hpp
	src/service/tokenizer.cpp
	src/service/tokens_oidc.hpp
	src/service/tokens_oidc.cpp
	src/service/tokens_session.hpp
	src/service/tokens_session.cpp
	src/service/oidc.hpp
	src/service/oidc.cpp
	src/service/service.hpp
	src/service/service.cpp
	src/service/session.hpp
	src/service/session.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql svetit::svetit)
target_include_directories(${PROJECT_NAME}_objs PUBLIC /usr/local/include)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

add_custom_command(
	OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/configs/session.key"
	COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/configs/create_session_key.sh"
)
add_custom_target(create-session-key
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/configs/session.key"
)
add_dependencies(${PROJECT_NAME} create-session-key)

# Unit Tests
add_executable(${PROJECT_NAME}_unittest
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple(
	PYTEST_ARGS --service-port=${SERVICE_PORT}
	REQUIREMENTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/requirements.txt"
)


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
	message(STATUS "Set install prefix: $ENV{PREFIX}")
	file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
	set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})

# Generate and install run scripts
set(CONFIG_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/static_config.yaml)
set(CONFIG_VARS_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/config_vars.yaml)
set(SESSION_KEY_PATH "${CMAKE_INSTALL_PREFIX}/session.key")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)
	set(CONFIG_VARS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars.yaml)
	set(SESSION_KEY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/session.key)
	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars_from_env.sh
		${CMAKE_CURRENT_BINARY_DIR}/config_vars_from_env.sh
		COPYONLY)
endif()
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configs/run_with_env.sh.in
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sh)
install(PROGRAMS
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sh
	${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars_from_env.sh
	DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
