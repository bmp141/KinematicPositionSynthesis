cmake_minimum_required(VERSION 3.20)
project(KinematicPositionSynthesis)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Charts)
find_package(GTest REQUIRED)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindowform.ui
    two_position_synthesis.cpp
    two_position_synthesis.hpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::Gui
    Qt6::Charts
    KinematicsLibrary
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

# Kinematics Unit Tests
add_executable(kinematics_unittests twopos_unittests.cpp)

target_sources(kinematics_unittests PRIVATE twopos_unittests.cpp)

target_include_directories(kinematics_unittests PRIVATE "${GTEST_INCLUDE_DIRS}")

target_link_libraries(kinematics_unittests
    PRIVATE
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
    )

# Kinematics Library
add_library(KinematicsLibrary SHARED)
target_sources(KinematicsLibrary
    PRIVATE
        two_position_synthesis.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS $(PROJECT_SOURCE_DIR)
        FILES
        two_position_synthesis.hpp
)
target_include_directories(KinematicsLibrary
    PRIVATE
        $(CMAKE_CURRENT_BINARY_DIR)
)

install(TARGETS KinematicsLibrary
    EXPORT kinematicslibraryTargets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)
