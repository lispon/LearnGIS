cmake_minimum_required(VERSION 3.5)

project(LearnGIS LANGUAGES CXX VERSION 0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(QGIS_DEMO_NAME qgis_demo)
add_subdirectory(QGIS_demo)

set(GDAL_DEMO_NAME gdal_demo)
add_subdirectory(GDAL_demo)
