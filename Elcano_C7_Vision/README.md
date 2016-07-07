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
- [wjwwood/serial](https://github.com/wjwwood/serial) (serialization over usb)
	- This is included in-tree in `serial-1.2.1/`
- [taywee/args](https://github.com/taywee/args) (argument parser)
	- This is included in-tree in `args.hh`

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

	--help     -h   Display the help dialouge
	--scale    -s   The scale of the images
	--cascade  -c   Location of the cascade classifier file
	--device   -d   The output device to send to
	--baudrate -b   The baudrate of the output device to send to

TODO
----

- Generate a classifier file for a cone
- Compute the 3D location of detected objects
