from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.descriptions import ParameterValue
import os

def generate_launch_description():
    # Path to your RViz config file (update to your actual path)
    rviz_config_path = os.path.join(
        os.getenv('HOME'),
        'ydlidar_ros2_ws/src/ydlidar_x3_ros/rviz',
        'my_lidar_config.rviz'
    )

    return LaunchDescription([
        # Start the LiDAR node
        Node(
            package='ydlidar_x3_ros',
            executable='ydlidar_node',
            name='ydlidar',
            output='screen'
        ),

        # Start RViz with your saved config
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rviz_config_path],
            output='screen'
        ),
    ])
