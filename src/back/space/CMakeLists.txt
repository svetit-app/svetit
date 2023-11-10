cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(svetit_space CXX)

option(SERVICE_PORT "Service port for listening" 8083)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
	src/api/info.cpp
	src/api/info.hpp
	src/api/invitation.cpp
	src/api/invitation.hpp
	src/api/link.cpp
	src/api/link.hpp
	src/api/list-available.cpp
	src/api/list-available.hpp
	src/api/list.hpp
	src/api/list.cpp
	src/api/space.hpp
	src/api/space.cpp
	src/api/user-manage.hpp
	src/api/user-manage.cpp
	src/api/user-list.hpp
	src/api/user-list.cpp

	src/repo/repository.cpp
	src/repo/repository.hpp
	src/repo/table_space.cpp
	src/repo/table_space.hpp
	src/repo/table_space_user.cpp
	src/repo/table_space_user.hpp
	src/repo/table_space_invitation.cpp
	src/repo/table_space_invitation.hpp
	src/repo/table_space_link.cpp
	src/repo/table_space_link.hpp

	src/model/space.hpp
	src/model/space_invitation.hpp
	src/model/space_link.hpp
	src/model/space_user.hpp
	src/model/space_serialize.hpp
	src/model/space_serialize.cpp
	src/model/invitation_serialize.hpp
	src/model/invitation_serialize.cpp
	src/model/link_serialize.hpp
	src/model/link_serialize.cpp
	src/model/user_serialize.hpp
	src/model/user_serialize.cpp
	src/model/errors.hpp
	src/model/space_service_info.hpp
	src/model/service_info_serialize.hpp
	src/model/service_info_serialize.cpp
	src/model/role.hpp
	src/model/role.cpp

	src/service/service.hpp
	src/service/service.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core userver-postgresql)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


# Unit Tests
add_executable(${PROJECT_NAME}_unittest

)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver-utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver-ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple(
	PYTEST_ARGS --service-port=${SERVICE_PORT}
	REQUIREMENTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/requirements.txt"
)


# Install
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
set(CONFIG_FALLBACK_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/dynamic_config_fallback.json)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configs/run_with_env.sh.in
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sh)
install(PROGRAMS
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sh
	${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars_from_env.sh
	DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
