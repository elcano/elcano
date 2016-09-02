Elcano Vision Subsystem
=======================

This is the subcomponent of the Elcano project that uses a Raspberry Pi
to scan the area for obstacles with OpenCV.

Dependencies
------------

- [meson](http://mesonbuild.com) and [ninja](https://ninja-build.org) (building)
- [bison](https://gnu.org/software/bison) and [flex](http://flex.sf.net) (generating the serial parser)
- [opencv](http://opencv.org) (object detection)
- [raspicam](https://github.com/cedricve/raspicam) (raspberry pi camera interface)
- [wjwwood/serial](https://github.com/wjwwood/serial) (serialization over usb)
	- This is included in-tree in `serial-1.2.1/`
- [taywee/args](https://github.com/taywee/args) (argument parser)
	- This is included in-tree in `args.hh`

Note that raspicam lacks a pkgconf file, so we manually create the
dependency in `meson.build`. Also, the serial dependency has a custom
`meson.build` file because the original project uses a bizare system
that I was having problems with. It may be a good idea to try and
upstream the build file.

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
- `arduino.*`: Oversees the communication between the Pi and the arduino(s)
	- `arduino.cc`: Code for writing data
	- `arduino.l`: Lexical scanner for the Elcano_Serial format
	- `arduino.y`: Semantic parser for the Elcano_Serial format
- `test-*.cc`: Test code for the specified file

Todo
----

- Generate a classifier file for a cone (`detect.cc`)
- The exact transformation between the input from localization and the output to the driver (`transform.cc`)
- Write a test suite for `detect.cc`
