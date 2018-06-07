Elcano Line Detection:

Updated 4/2/2018
Author: Conor Van Achte

How to use:
1. Make sure the Raspi Camera is detected by the OS by running this 
   command in console "vcgencmd get_camera".
2. If the camera is connected, you may need to enable the camera for use.
   This can be done by entering the command "sudo modprobe bcm2835-v4l2".
   This command opens up the camera for use by the OS.
3. Compile the program by running entering "g++ -std=c++11 $(pkg-config --libs --cflags opencv) -o LineTest LineDetectionMain.cpp" in console.
   (Optional) To run the program with communication to arduino add rs232.c to the compilation string.
4. Run the program with ./LineTest.
5. Exit the program by pressing the ESC key.

How does it work?

The Elcano Line Detection works by use of the Raspi Camera on the Raspi. As of 4/2/2018, the Line Detection code detects yellow lines, and 
has a filter for yellow colors. The program takes a frame as input from the camera, uses a yellow filter mask
on the image to isolate the yellow colors, and then applies a Gaussian blur to smooth out the image.
A Canny edge detector is then applied to the frame to detect the edges found in the image.
Finally a HoughLine transform is performed to draw detect lines on the image. The Elcano Line Detection program will then find the average
of the lines found, and draw an averaged line on the frame every 5 frames.This code is currently in testing
and will need refactoring and possible a new implementation for full lane detection.

Update 05/03/2018

Elcano Line Detection code has been refactored and appears to be working reasonably well with the arduino sketch Kyle Farmer wrote. The Line Detection
program is running much smoother after a reduction in GaussianBlur from a 25x25 matrix down to a 9x9 and finally to a 3x3. The Pi sends information to the
arduino mega, which then provides instructions to the servomotor on which direction to turn. Currently we have a Raspi connected to
an arduino mega via serial connection, and the arduino mega is connected to a small servo motor that rotates based on the values found by the raspi.

Additional Information:

The Elcano Line Detection code was written using Visual Studio. In order to compile you may need to install the additional libraries provided by
OpenCV. A link to setup Visual Studio with OpenCV can be found here: https://www.youtube.com/watch?v=l4372qtZ4dc.