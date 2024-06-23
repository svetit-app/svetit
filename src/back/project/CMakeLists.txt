cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(svetit_project CXX)

set(SERVICE_PORT 8084 CACHE INT "Service port for listening")

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


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
	third_party/svetit/shared/errors.hpp
	third_party/svetit/shared/errors.cpp
	third_party/svetit/shared/errors_catchit.hpp
	third_party/svetit/shared/errors_catchit.cpp
	third_party/svetit/shared/headers.hpp
	third_party/svetit/shared/paging.hpp
	third_party/svetit/shared/paging_serialize.hpp
	third_party/svetit/shared/paging.cpp
	third_party/svetit/shared/parse/request.hpp
	third_party/svetit/shared/parse/request.cpp
	third_party/svetit/shared/parse/uuid.hpp
	third_party/svetit/shared/parse/uuid.cpp
	third_party/svetit/shared/strings/camel2snake.hpp
	third_party/svetit/shared/strings/camel2snake.cpp
	third_party/svetit/shared/db/db_base.hpp
	third_party/svetit/shared/db/db_base.cpp
	third_party/svetit/shared/schemas.hpp
	third_party/svetit/shared/schemas.cpp


	src/repo/repository.cpp
	src/repo/repository.hpp

	src/service/service.hpp
	src/service/service.cpp

	src/api/details.hpp
	src/api/details.cpp
	src/api/cc-mode-type-list.cpp
	src/api/cc-mode-type-list.hpp

	src/model/sync_direction.hpp
	src/model/sync_direction.cpp
	src/model/project.hpp
	src/model/project_serialize.hpp
	src/model/project_serialize.cpp
	src/model/param_value_type.hpp
	src/model/param_value_type.cpp
	src/model/param_type.hpp
	src/model/param_type_serialize.hpp
	src/model/param_type_serialize.cpp
	src/model/project_param.hpp
	src/model/project_param_serialize.hpp
	src/model/project_param_serialize.cpp
	src/model/section.hpp
	src/model/section_serialize.hpp
	src/model/section_serialize.cpp
	src/model/section_param.hpp
	src/model/section_param_serialize.hpp
	src/model/section_param_serialize.cpp
	src/model/cc_type.hpp
	src/model/cc_type_serialize.hpp
	src/model/cc_type_serialize.cpp
	src/model/control_circuit.hpp
	src/model/control_circuit_serialize.hpp
	src/model/control_circuit_serialize.cpp
	src/model/plugin.hpp
	src/model/plugin_serialize.hpp
	src/model/plugin_serialize.cpp
	src/model/device.hpp
	src/model/device_serialize.hpp
	src/model/device_serialize.cpp
	src/model/device_plugin_param.hpp
	src/model/device_plugin_param_serialize.hpp
	src/model/device_plugin_param_serialize.cpp
	src/model/code.hpp
	src/model/code_serialize.hpp
	src/model/code_serialize.cpp
	src/model/measure.hpp
	src/model/measure_serialize.hpp
	src/model/measure_serialize.cpp
	src/model/save_timer.hpp
	src/model/save_timer_serialize.hpp
	src/model/save_timer_serialize.cpp
	src/model/cc_type_param.hpp
	src/model/cc_type_param_serialize.hpp
	src/model/cc_type_param_serialize.cpp
	src/model/save_algorithm.hpp
	src/model/save_algorithm.cpp
	src/model/di_mode.hpp
	src/model/di_mode.cpp
	src/model/di_type.hpp
	src/model/di_type_serialize.hpp
	src/model/di_type_serialize.cpp
	src/model/di_plugin_param.hpp
	src/model/di_plugin_param_serialize.hpp
	src/model/di_plugin_param_serialize.cpp
	src/model/cc_type_di_type.hpp
	src/model/cc_type_di_type_serialize.hpp
	src/model/cc_type_di_type_serialize.cpp
	src/model/device_item.hpp
	src/model/device_item_serialize.hpp
	src/model/device_item_serialize.cpp
	src/model/cc_mode_type.hpp
	src/model/cc_mode_type_serialize.hpp
	src/model/cc_mode_type_serialize.cpp
	src/model/cc_di.hpp
	src/model/cc_di_serialize.hpp
	src/model/cc_di_serialize.cpp
	src/model/cc_param.hpp
	src/model/cc_param_serialize.hpp
	src/model/cc_param_serialize.cpp
	src/model/cc_status_category.hpp
	src/model/cc_status_category_serialize.hpp
	src/model/cc_status_category_serialize.cpp
	src/model/cc_status_type.hpp
	src/model/cc_status_type_serialize.hpp
	src/model/cc_status_type_serialize.cpp
	src/model/value_view.hpp
	src/model/value_view_serialize.hpp
	src/model/value_view_serialize.cpp
	src/model/translation.hpp
	src/model/translation_serialize.hpp
	src/model/translation_serialize.cpp

	src/repo/table_project.hpp
	src/repo/table_project.cpp
	src/repo/table_param_type.hpp
	src/repo/table_param_type.cpp
	src/repo/table_project_param.hpp
	src/repo/table_project_param.cpp
	src/repo/table_section.hpp
	src/repo/table_section.cpp
	src/repo/table_section_param.hpp
	src/repo/table_section_param.cpp
	src/repo/table_cc_type.hpp
	src/repo/table_cc_type.cpp
	src/repo/table_control_circuit.hpp
	src/repo/table_control_circuit.cpp
	src/repo/table_plugin.hpp
	src/repo/table_plugin.cpp
	src/repo/table_device.hpp
	src/repo/table_device.cpp
	src/repo/table_device_plugin_param.hpp
	src/repo/table_device_plugin_param.cpp
	src/repo/table_code.hpp
	src/repo/table_code.cpp
	src/repo/table_measure.hpp
	src/repo/table_measure.cpp
	src/repo/table_save_timer.hpp
	src/repo/table_save_timer.cpp
	src/repo/table_cc_type_param.hpp
	src/repo/table_cc_type_param.cpp
	src/repo/table_di_type.hpp
	src/repo/table_di_type.cpp
	src/repo/table_di_plugin_param.hpp
	src/repo/table_di_plugin_param.cpp
	src/repo/table_cc_type_di_type.hpp
	src/repo/table_cc_type_di_type.cpp
	src/repo/table_device_item.hpp
	src/repo/table_device_item.cpp
	src/repo/table_cc_mode_type.hpp
	src/repo/table_cc_mode_type.cpp
	src/repo/table_cc_di.hpp
	src/repo/table_cc_di.cpp
	src/repo/table_cc_param.hpp
	src/repo/table_cc_param.cpp
	src/repo/table_cc_status_category.hpp
	src/repo/table_cc_status_category.cpp
	src/repo/table_cc_status_type.hpp
	src/repo/table_cc_status_type.cpp
	src/repo/table_value_view.hpp
	src/repo/table_value_view.cpp
	src/repo/table_translation.hpp
	src/repo/table_translation.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)

target_include_directories(${PROJECT_NAME}_objs PUBLIC third_party/svetit)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


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
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)
	set(CONFIG_VARS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars.yaml)
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
