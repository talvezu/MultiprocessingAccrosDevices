set(CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/client.cpp")

add_executable(${CLIENT_EXECUTABLE_NAME} ${CLIENT_SOURCES})
message("RapidJSON is a header-only library, including no linking")
include_directories(${RapidJSON_SOURCE_DIR}/include
                    ${cxxopts_SOURCE_DIR}/include)

add_subdirectory(client_utils)

target_link_libraries(
    ${CLIENT_EXECUTABLE_NAME}
    PRIVATE spdlog::spdlog
            cxxopts::cxxopts
            ${CLIENT_UTILS_NAME})

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${CLIENT_EXECUTABLE_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${CLIENT_EXECUTABLE_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${CLIENT_EXECUTABLE_NAME})
endif()
