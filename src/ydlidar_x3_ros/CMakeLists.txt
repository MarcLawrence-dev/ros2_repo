cmake_minimum_required(VERSION 3.5)
project(ydlidar_x3_ros)

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

# --- Point to the YDLidar SDK ---
set(YDLIDAR_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../YDLidar-SDK)

include_directories(
 /home/ankledogg/ydlidar_ros2_ws/src/YDLidar-SDK/src
  /home/ankledogg/ydlidar_ros2_ws/src/YDLidar-SDK/core
)

link_directories(
  ${YDLIDAR_SDK_DIR}/build
)

# --- Node executable ---
add_executable(ydlidar_x3_node src/ydlidar_x3_node.cpp)

ament_target_dependencies(ydlidar_x3_node
  rclcpp
  sensor_msgs
)

# Link against the YDLidar SDK
target_link_libraries(ydlidar_x3_node
  ydlidar_sdk
)

install(TARGETS
  ydlidar_x3_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
