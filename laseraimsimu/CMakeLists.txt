cmake_minimum_required(VERSION 3.5)

project(laseraimsimu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(laseraimsimu main.cpp)

target_link_libraries(laseraimsimu ${OpenCV_LIBS})

include(GNUInstallDirs)
install(TARGETS laseraimsimu
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
