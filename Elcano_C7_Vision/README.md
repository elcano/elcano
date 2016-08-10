Elcano Vision Subsystem
=======================

This is the subcomponent of the Elcano project that uses a Raspberry Pi
to scan the area for obstacles with OpenCV.

Dependencies
------------

- [meson](http://mesonbuild.com) (building)
- [ninja](https://ninja-build.org) (building)
- [opencv](http://opencv.org) (object detection)
- [raspicam](https://github.com/cedricve/raspicam) (raspberry pi camera interface)
- [wjwwood/serial](https://github.com/wjwwood/serial) (serialization over usb)
	- This is included in-tree in `serial-1.2.1/`
- [taywee/args](https://github.com/taywee/args) (argument parser)
	- This is included in-tree in `args.hh`

Note that raspicam lacks a pkgconf file, so we manually link inside
meson.build. If this changes in the future, change it here.

Build Instructions
------------------

	mkdir build
	meson.py build
	cd build
	ninja

Usage
-----

The main executable file is called `elcano-pi`, and it uses these options:

	--help     -h   Display the help dialouge
	--scale    -s   The scale of the images
	--cascade  -c   Location of the cascade classifier file
	--device   -d   The output device to send to
	--baudrate -b   The baudrate of the output device to send to

Developer Overview
------------------

- `main.cc`: The driver function
- `detect.cc`: Uses OpenCV to detect objects with the camera
- `transform.cc`: Computes transformations between the world and a photo
- `arduino.cc`: Oversees the communication between the Pi and the arduino(s)
- `test-*.cc`: Test code for the specified file

Todo
----

- Generate a classifier file for a cone
- The exact transformation between the input from localization and the output to the driver (`transform.cc`)
- Parse input/write output in the correct format for `[project root]/libraries/Elcano_Serial`
- Write test suite for `arduino.cc`
