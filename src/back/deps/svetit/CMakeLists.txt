cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(svetit
	VERSION 1.0.0
	DESCRIPTION
		"Svetit common utils library"
	LANGUAGES CXX)

file(GLOB_RECURSE SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE UNIT_TEST_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*_test.cpp
)
file(GLOB_RECURSE BENCH_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*_benchmark.cpp
)
list(REMOVE_ITEM SOURCES ${UNIT_TEST_SOURCES} ${BENCH_SOURCES})

find_package(userver COMPONENTS core postgresql REQUIRED)

userver_setup_environment()

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_DEBUG_POSTFIX d)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC userver::postgresql)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Unit tests
add_executable(${PROJECT_NAME}_unittest ${UNIT_TEST_SOURCES})
target_include_directories (${PROJECT_NAME}_unittest SYSTEM PRIVATE
	$<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>
)
target_link_libraries(${PROJECT_NAME}_unittest
	PRIVATE userver::utest
	PUBLIC ${PROJECT_NAME}
)
add_google_tests(${PROJECT_NAME}_unittest)

# Benchmark tests
add_executable(${PROJECT_NAME}_benchmark ${BENCH_SOURCES})
target_include_directories (${PROJECT_NAME}_benchmark SYSTEM PRIVATE
	$<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>
)
target_link_libraries(${PROJECT_NAME}_benchmark
	PRIVATE userver::ubench
	PUBLIC ${PROJECT_NAME}
)

add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

write_basic_package_version_file(
	"${PROJECT_NAME}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY ExactVersion)

configure_package_config_file(
	"${CMAKE_CURRENT_LIST_DIR}/cmake/config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
