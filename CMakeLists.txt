cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(YOU_VERSION 0.139.0)

project(
  You
  VERSION ${YOU_VERSION}
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
  set(CMAKE_OBJECT_PATH_MAX 500)
endif ()
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)
qt_standard_project_setup()
set(WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(NOT QATERIAL_CACHE_QML)
  set(QATERIAL_CACHE_QML "NO_CACHEGEN")
else ()
  set(QATERIAL_CACHE_QML "")
endif()

qt_add_qml_module(You
        URI "You"
        VERSION 1.0
        OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/You
        QML_FILES
        src/You/Components/TBD.qml

)
add_library(You::You ALIAS You)
# ───── LIBRARY TESTS ─────
if(YOU_TEST)
  if(YOU_VERBOSE)
    message(STATUS "building tests")
  endif()
  include(CTest)
  add_subdirectory(tests)
  add_custom_target(
    tidy
    COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR}
    COMMENT "cpp linter")
endif()
