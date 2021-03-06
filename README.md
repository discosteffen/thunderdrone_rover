# ThunderDrone Rover

## Technical Specs
### Compute Power
* [Nvidia Jetson TX2](http://www.nvidia.com/object/embedded-systems-dev-kits-modules.html)

### Software
* Robot Operating System (ROS)
* Yolo2 (Real Time Object Detection)

### Sensors
* [RPLidar A2](https://www.slamtec.com/en/Lidar)
* [Logitech - c920 Pro Webcam](https://www.logitech.com/en-us/product/hd-pro-webcam-c920)
* Inertial Measurement Unit

### Misc Hardware

## Setup

### Jetson Setup
Kernel configuration for RPlidar

### Frontend
To run frontend web output locate the frontend folder and run local server of choice.
Python example: python -m SimpleHTTPServer 8000

### ROS

#### Lidar Output Example Hector Mapping
![ThunderDrone Map](thunder_drone_rodeo_demo_2017_10_cropped.jpg)

Github includes three ROS folders.

### Speed Controller
smc_linux for Pololu setup


