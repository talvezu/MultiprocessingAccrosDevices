# Sources and Headers
set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/utils.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

# Find the thread library
find_package(Threads REQUIRED)

# Library
add_library(${CLIENT_UTILS_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${CLIENT_UTILS_NAME} PUBLIC ${LIBRARY_INCLUDES})
target_link_libraries(
    ${CLIENT_UTILS_NAME}
    PRIVATE nlohmann_json::nlohmann_json
            Threads::Threads
            fmt::fmt
            spdlog::spdlog
            cxxopts::cxxopts)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${CLIENT_UTILS_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${CLIENT_UTILS_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${CLIENT_UTILS_NAME})
endif()
