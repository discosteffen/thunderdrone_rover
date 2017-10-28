cmake_minimum_required(VERSION 2.8.3)
project(rplidar_ros_gaps)

set(RPLIDAR_SDK_PATH "./sdk/")
set(GAPS_PATH "./gaps/")

FILE(GLOB RPLIDAR_SDK_SRC 
  "${RPLIDAR_SDK_PATH}/src/arch/linux/*.cpp"
  "${RPLIDAR_SDK_PATH}/src/hal/*.cpp"
  "${RPLIDAR_SDK_PATH}/src/*.cpp"
  "${GAPS_PATH}/src/*.cpp"
)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosconsole
  sensor_msgs
)

include_directories(
  ${RPLIDAR_SDK_PATH}/include
  ${RPLIDAR_SDK_PATH}/src
  ${GAPS_PATH}/include
  ${catkin_INCLUDE_DIRS}
)

catkin_package()

add_executable(rplidarGapsNode src/node.cpp ${RPLIDAR_SDK_SRC})
target_link_libraries(rplidarGapsNode ${catkin_LIBRARIES})

add_executable(rplidarGapsNodeClient src/client.cpp)
target_link_libraries(rplidarGapsNodeClient ${catkin_LIBRARIES})

install(TARGETS rplidarGapsNode rplidarGapsNodeClient
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch rviz sdk
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  USE_SOURCE_PERMISSIONS
)
