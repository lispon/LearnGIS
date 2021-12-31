# QGIS_DEMO_NAME = qgis_demo

find_package(Qt5 COMPONENTS Widgets Xml Svg LinguistTools REQUIRED)

file(GLOB_RECURSE H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
file(GLOB_RECURSE TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.ts)

add_executable( ${QGIS_DEMO_NAME}
    ${H_FILES}
    ${CPP_FILES}
    ${UI_FILES}
    ${TS_FILES}
)

#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

if( UNIX )
    set(CMAKE_MODULE_PATH "/usr/share/qgis;${CMAKE_MODULE_PATH}")
elseif( WIN32 )
    # using your install osgeo4w path.
    set(OSGEO4W_ROOT "E:/OSGeo4W")
    set(OSGEO4W_QGIS_SUBDIR "qgis-ltr-dev")


    set(QGIS_INCLUDE_DIR "${OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR}/include")
    set(QGIS_FOUND "${OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR}/lib")
    set(CMAKE_MODULE_PATH "${OSGEO4W_ROOT}/apps/${OSGEO4W_QGIS_SUBDIR};${CMAKE_MODULE_PATH}")
endif()

find_package(QGIS COMPONENTS REQUIRED)

target_compile_definitions( ${QGIS_DEMO_NAME}
        PRIVATE
    -D_USE_MATH_DEFINES
)

target_include_directories( ${QGIS_DEMO_NAME}
        PRIVATE
    ${QGIS_INCLUDE_DIR}
)

target_link_libraries( ${QGIS_DEMO_NAME}
        PRIVATE
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg

    qgis_app
    qgis_core
    qgis_gui
)

# copy map_data to bianry dir.
#configure_file(${CMAKE_SOURCE_DIR}/map_data/SCSENC_ed2_reissue2/ENC_ROOT/EA200001.000 ${CMAKE_CURRENT_BINARY_DIR}/mapdata/SCSENC_ed2_reissue2/ENC_ROOT/EA200001.000 COPYONLY)

file(   COPY
    ${CMAKE_SOURCE_DIR}/map_data/s57
    ${CMAKE_SOURCE_DIR}/map_data/shp
        DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/mapdata
)
