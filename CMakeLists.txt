# Copyright: (C) 2016 iCub Facility - Fondazione Istituto Italiano di Tecnologia
# Authors: Francesco Romano <francesco.romano@iit.it>
# CopyPolicy: Released under the terms of the GNU LGPL v2.0+

cmake_minimum_required(VERSION 2.8.9)
project(xsens-mvn)

# Add local CMake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

# Find YARP and include the CMake code to compile plugins
find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
include(YarpPlugin)
include(YarpInstallationHelpers)
set(YARP_FORCE_DYNAMIC_PLUGINS TRUE)
yarp_configure_external_installation(${PROJECT_NAME} WITH_PLUGINS)

# Compile the plugins by default
set(COMPILE_BY_DEFAULT ON)
find_package(XsensXME REQUIRED)

yarp_prepare_device(xsens-mvn TYPE yarp::dev::XsensMVN
                                   INCLUDE include/XsensMVN.h)

yarp_add_plugin(xsens-mvn   src/XsensMVN.cpp
							src/XsensMVNPrivate.cpp
							include/XsensMVN.h
							include/XsensMVNPrivate.h)

include_directories(include)
include_directories(SYSTEM ${YARP_INCLUDE_DIRS})
target_link_libraries(xsens-mvn YARP::YARP_OS YARP::YARP_dev YARP::YARP_sig ${XsensXME_LIBRARIES})

# yarp_install(TARGETS xsens-mvn
             # COMPONENT runtime
             # LIBRARY DESTINATION ${XSENS_MVN_DYNAMIC_PLUGINS_INSTALL_DIR}
             # ARCHIVE DESTINATION ${XSENS_MVN_STATIC_PLUGINS_INSTALL_DIR})
# yarp_install(FILES xsens-mvn.ini  DESTINATION ${XSENS_MVN_PLUGIN_MANIFESTS_INSTALL_DIR})


