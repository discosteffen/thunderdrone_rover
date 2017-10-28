RPLIDAR ROS GAPS package
=====================================================================


Based on RPLIDAR ROS by General Algorithmics (R)


ROS node and test application for RPLIDAR

Visit following Website for more details about RPLIDAR:

rplidar roswiki: http://wiki.ros.org/rplidar

rplidar HomePage:   http://www.slamtec.com/en/Lidar

rplidar Tutorial:  https://github.com/robopeak/rplidar_ros/wiki

How to build rplidar ros package
=====================================================================
    1) Clone this project to your catkin's workspace src folder
    2) Running catkin_make to build rplidarGapsNode and rplidarGapsNodeClient

How to run rplidar ros package
=====================================================================
There're two ways to run rplidar ros package

I. Run rplidar node and view in the rviz
------------------------------------------------------------
roslaunch rplidar_ros_gaps view_rplidar_gaps.launch

You should see rplidar's scan result in the rviz.

II. Run rplidar node and view using test application
------------------------------------------------------------
roslaunch rplidar_ros_gaps rplidar_gaps.launch

rosrun rplidar_ros_gaps rplidarGapsNodeClient

You should see rplidar's scan result in the console

RPLidar frame
=====================================================================
RPLidar frame must be broadcasted according to picture shown in
rplidar-frame.png
