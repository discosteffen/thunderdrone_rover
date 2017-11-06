# Install script for directory: /home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/src/rplidar_ros_gaps

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/build/rplidar_ros_gaps/catkin_generated/installspace/rplidar_ros_gaps.pc")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rplidar_ros_gaps/cmake" TYPE FILE FILES
    "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/build/rplidar_ros_gaps/catkin_generated/installspace/rplidar_ros_gapsConfig.cmake"
    "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/build/rplidar_ros_gaps/catkin_generated/installspace/rplidar_ros_gapsConfig-version.cmake"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rplidar_ros_gaps" TYPE FILE FILES "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/src/rplidar_ros_gaps/package.xml")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps" TYPE EXECUTABLE FILES "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/devel/lib/rplidar_ros_gaps/rplidarGapsNode")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode"
         OLD_RPATH "/opt/ros/lunar/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNode")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps" TYPE EXECUTABLE FILES "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/devel/lib/rplidar_ros_gaps/rplidarGapsNodeClient")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient"
         OLD_RPATH "/opt/ros/lunar/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/rplidar_ros_gaps/rplidarGapsNodeClient")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rplidar_ros_gaps" TYPE DIRECTORY FILES
    "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/src/rplidar_ros_gaps/launch"
    "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/src/rplidar_ros_gaps/rviz"
    "/home/steffen/Dev/sofwerx/thunderdrone/ros/catkin_ws/src/rplidar_ros_gaps/sdk"
    USE_SOURCE_PERMISSIONS)
endif()

