Elcano Vision Subsystem
=======================

This is the subcomponent of the Elcano project that uses a Raspberry Pi
to scan the area for obstacles with OpenCV.

Dependencies
------------

- [opencv (v3.2)](http://opencv.org) (object detection)
- [raspicam](https://github.com/cedricve/raspicam) (raspberry pi camera interface)


Build Instructions
------------------

1. Compile rs232:
	gcc -c rs232.c -o rs232.o

2. Compile main:
	g++ -std=c++11 $(pkg-config --libs --cflags opencv) -o main main.cpp rs232.o




Developer Overview
------------------

- `main.cpp`: The driver function
- `rs232.c`: Allows serial communication between Raspberry Pi and Arduino


Todo
----

- Improve reliability/accuracy of cone location with Kalman Filter
- Add makefile for building
- Add comments to main
