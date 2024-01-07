find_package(Qt6 REQUIRED COMPONENTS Test Core Network)
find_package(Catch2 REQUIRED COMPONENTS Catch2)
include(Catch)
set(CMAKE_OBJECT_PATH_MAX 9999)

add_library(utils_for_tests
        testutils.cpp testutils.hpp
)
target_link_libraries(utils_for_tests
        PUBLIC
        Qt6::Quick
        Qt6::Test
        Qt6::Qml
        Catch2::Catch2
)

add_executable(tests
        main.cpp
        test_foo.cpp

)
target_link_libraries(tests
        PUBLIC
        Qt6::Test
        Qt6::Qml
        You::You
        Catch2::Catch2
        utils_for_tests
)
catch_discover_tests(tests
        DL_PATHS ${QT_DL_LIBRARIES}
)
