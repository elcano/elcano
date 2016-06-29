Elcano Vision Library
=====================

This is a simple project for using OpenCV object detection on a
Raspberry Pi.

Dependencies
------------

- [meson](http://mesonbuild.com) (building)
- [ninja](https://ninja-build.org) (building)
- [opencv](http://opencv.org) (object detection)
- [raspicam](https://github.com/cedricve/raspicam) (raspberry pi camera interface)

Note that raspicam lacks a pkgconf file, so we manually link inside
meson.build. If this changes in the future changes it here.

Build Instructions
------------------

	mkdir build
	meson.py build
	cd build
	ninja

Usage
-----

	--help -h   	Display the help dialouge
	--scale -s  	The scale of the images
	--cascade -c	Location of the cascade classifier file

TODO
----

- Generate a classifier file for a cone
- Compute the 3D location of detected objects
- Communicate this data over the pins to the pilot/mapping device
- Review and remove old documentation

Old Documentation
-----------------

C7: Vision Data Management and
S4: Smart camera and
C5: Obstacle Detection.

Vision is one of the more difficult pieces of an autonomous vehicle.
If a suitable low cost system can be purchased, that can be a more robust
method than writing our own software.

The original design was to use the Coyote Robotics camera for S4.
That device contains a stereo camera pair, circuitry and firmware.
The interface that it presents to C7 is a serial stream of information.
That information includes:
Bearings and distances to detected objects.
Speeds of detected objects.
Lane following information: distance to center line or side line.
Visual odometry.
The target price of the Coyote Robotics camera was $400. 
They seem to have decided not to manufacture it.

C7 has the job of sorting out the information stream and passing visual odometry and
lane following information to the C6 Navigator and passing obstacle information to
C5 Obstacle detection.  It may make sense to combine modules C7 and C5 in a single
hardware device.

There are several alternatives:
Locate another product that performs these functions.
License Coyote Robotics IP and use it.
Buy something like Sony's smart camera and program vision algorithms.
Use simple sensors to substitute for vision.

If we write our own software, S4 and C7 can be the same device.

Vision is a lower priority.  
The first prototype can be blind and assume that there are no obstacles.
We can then evaluate what cameras are on the market.

Simple sensors:
The three visual tasks are
1) Optical Odometry.
An optical mouse works by taking a low resolution image of the desk top and
putting it though a DSP to correlate the textures in one frame with the next.
This enables it to sense how far it has moved in two orthogonal directions. Mouse
cameras are about 30 x 30 pixels and run at several kHz. The image is about 1 mm
on an edge and depth of field is also about 1 mm. It sould be possible to replace the
mouse's optics so that it is positioned about a meter above the road and has a
wider depth of field.  The speed would be appropriate for land vehicles.
The mouse chip probably does not have to be reprogrammed; this is an optical task.
A camera does visual odometry by pointing ahead instead of down. It transforms the
projection of the road surface so that two frames can be correlated and distance
traveled determined.
2) Lane following
Line following is commonly done in robot competitions. A few light sensors can 
accomplish the task. A simple camera can be programmed to provide this information.
3) Obstacle detection
Several low cost ranging sensors can determine whether an obstacle is present on a
particular bearing. C5 has the job of checking on these sensors.  The sensors may be
binary and return a clear/not clear result on a bearing or they may be analog and report
the distance to the object.  The technology can be ultrasound or laser.  Ultrasound times
the return of an echo.  Lasers can work by time of flight, but this is more expensive since
light is so fast. A less expensive laser technique is Laser Optical Triangulation (LOT).
A laser spot is sent to a target and the reflection is found on a receiver. There is a slight
angle between the laser and receiver and the farther the two are separated, the longer range
is possible. The position of the returned spot can be used to measure the distance to
the object.