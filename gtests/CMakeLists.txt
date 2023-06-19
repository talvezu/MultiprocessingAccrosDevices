if(ENABLE_TESTING)

    #### NOTE ####
    # due to error such as
    # /usr/bin/ld: ../client/client_utils/libclient_utils.a: error adding symbols: file format not recognized
    # files from client/client_utils are read directly,
    # same for linking fmt::fmt and spdlog::spdlog
    ##############
    set(GTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/gtest_timing.cpp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/gtest_mean_stdv.cpp"
                      "../client/client_utils/utils.cpp")
    add_executable(${UNIT_GTEST_NAME} ${GTEST_SOURCES})

    #already included from rapid jason
    if(USE_CONAN)
        include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
        find_package(googletest REQUIRED)
    elseif(USE_VCPKG)
        include(${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake)
        find_package(googletest REQUIRED)
    else(USE_CPM)
        #include(CPM)
        #cpmaddpackage("gh:google/googletest@1.13.0")
    endif()

    #workaround
    set(CLIENT_UTILS_DIRECTORY_INCLUDES "../client")

    target_include_directories(${UNIT_GTEST_NAME} PUBLIC ${RapidJSON_SOURCE_DIR}/include ${CLIENT_UTILS_DIRECTORY_INCLUDES})

    #### NOTE2 ####
    #  due to gtest in _deps/rapidjson-src/thirdparty/gtest/googletest/CMakeLists.txt +92
    # target_link_libraries(${UNIT_GTEST_NAME} PRIVATE  GTest::gtest_main) is not used
    ################

    # Find the thread library
    find_package(Threads REQUIRED)
    target_link_libraries(${UNIT_GTEST_NAME} PRIVATE  gtest_main gtest fmt::fmt spdlog::spdlog Threads::Threads)
    #target_link_libraries(${UNIT_GTEST_NAME} PRIVATE  ${CLIENT_UTILS_NAME})



endif()
