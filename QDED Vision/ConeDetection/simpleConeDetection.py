#!/usr/bin/python
# --------------------------------------------------------------
# singleConeDetection.py
# This script is adapted from code by Adrian Rosebrock at http://www.pyimagesearch.com
#
# This class demonstrates both video IO threading and OpenCV functions to detect a traffic cone.
# Cone range and angle data is based on camera-specific values and these 
# should be tested and adjusted for accuracy
#
# Cone color values depend on camara and lighting and may need adjustment
#
# Language and formatting notes: Use Python 2 only. This file uses spaces for indentation and LF line endings
#
# --------------------------------------------------------------

# import the necessary packages
from collections import deque
import numpy as np
import argparse
#import imutils
import cv2
import time
from threading import Thread

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
    help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=5,
    help="center point memory (# points) and line thickness")
ap.add_argument("-H", "--hsv",
    help="use HSV color space, requires software conversion (default: use BGR)",
    action="store_true")
ap.add_argument("-G", "--gui",
    help="display GUI elements (default: no GUI)",
    action="store_true")
ap.add_argument("-V", "--verbose",
    help="write outputs to console (default: no output in console)",
    action="store_true")
args = vars(ap.parse_args())

class WebcamVideoStream:
    def __init__(self, src=0):
        # initialize the video camera stream and read the first frame
        # from the stream
        self.stream = cv2.VideoCapture(src)
        (self.grabbed, self.frame) = self.stream.read()
 
        # initialize the variable used to indicate if the thread should
        # be stopped
        self.stopped = False

    def start(self):
        # start the thread to read frames from the video stream
        Thread(target=self.update, args=()).start()
        return self
 
    def update(self):
        # keep looping infinitely until the thread is stopped
        while True:
            # if the thread indicator variable is set, stop the thread
            if self.stopped:
                return
 
            # otherwise, read the next frame from the stream
            (self.grabbed, self.frame) = self.stream.read()
 
    def read(self):
        # return the frame most recently read
        return (self.frame, self.grabbed)
 
    def stop(self):
        # indicate that the thread should be stopped
        self.stopped = True
# end of class

# numbers are HSV
# in openCV, Hue range is [0-179], Saturation range is [0-255] and Value range is [0-255]
# I am using inverted RGB converted to HSV, so these HSV values are actually for "blue"
orangeLowerHSV = np.array([86,63,63])
orangeUpperHSV = np.array([99,255,225])
# numbers are RGB (red, green, blue)
orangeLowerRGB = np.array([16,32,160])
orangeUpperRGB = np.array([255,142,229])
# conversion from RGB <-> HSV doesn't work here for some reason, it creates datatype errors

coneHeightM = 0.4953 # 0.4953 meters = 19.5 inches; this is specific to a particular traffic cone
camConstant = 1583.5 # equal to FL m / PS m; this is specific to an individual camera
distConstant = coneHeightM * camConstant # range (m) = dC / N (px)
degPerPx = 0.09 # this is NOT accurate towards edge of frame
imgCenterX = 640/2

coneLocationString = "no cone location data yet..."

# font variables
fontFace = 0 # this is an enum for a font in openCV
fontScale = 1.0

# timing variables
loopStartTime = time.time()
loopEndTime = time.time()
duration = time.time()
totalTime = time.time()
coneFrames = 0.0
frames = 0.0
meanTime = 0.0

# if a video file path was not supplied, grab the reference
# to the webcam
src = 0
if args.get("video"):
    # use a video file, not webcam
    src = args["video"]
vs = WebcamVideoStream(src).start()

# keep looping
startTime = time.time()
while True:
    try:
        #times the loop in seconds
        loopStartTime = time.time()
        # grab the current frame
    ##    grabbed, img = camera.read()
    ##    cv2.imshow('image', img)
        (frame, grabbed) = vs.read()


        # if we are viewing a video and we did not grab a frame,
        # then we have reached the end of the video
        if args.get("video") and not grabbed:
            break
        if args.get("hsv"):
            # HSV conversion
            frame = cv2.bitwise_not(frame) #inverts RGB
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # construct a mask for the color, then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        if args.get("hsv"):
            # use HSV;
            mask = cv2.inRange(hsv, orangeLowerHSV, orangeUpperHSV)
            mask = cv2.erode(mask, None, iterations=1)
            mask = cv2.dilate(mask, None, iterations=1)
        else:
            # use RGB
            mask = cv2.inRange(frame, orangeLowerRGB, orangeUpperRGB)
            #mask = cv2.erode(mask, None, iterations=1)
            #mask = cv2.dilate(mask, None, iterations=1)

        # find contours in the mask and initialize the current
        # (x, y) center of the cone
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        # only proceed if at least one contour was found
        if len(cnts) > 0:
            coneFrames = coneFrames + 1
            # print("found contours...")
            c = max(cnts, key=cv2.contourArea)
            x,y,w,h = cv2.boundingRect(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
            # draw the rectangle enclosing the contour
            # then update the list of tracked points
            cv2.rectangle(frame,(x,y),(x+w,y+h),(204,0,102),4)
            distance = distConstant / h
            degrees = degPerPx * (center[0] - imgCenterX)
            coneLocation = {degrees, distance} # TODO: write this to UART
            coneLocationString = "Angle: {0:.1f} (deg), distance: {1:.2f} (m)".format(degrees, distance)

        if args.get("gui"):
            cv2.putText(frame, coneLocationString, (5,15), fontFace, 0.5,(255,255,255,255), 1)

            cv2.imshow("Frame", frame)

        key = cv2.waitKey(1) & 0xFF
        # if the 'q' key is pressed, stop the loop
        if key == ord("q"):
            break

        loopEndTime = time.time()
        duration = loopEndTime - loopStartTime
        frames = frames + 1
        if args.get("verbose"):
            print("FPS", np.divide(1.0, duration), coneLocationString)
    except KeyboardInterrupt:
    # if keyboard inturrupt (Crtl+C) stop loop
        break
totalTime = time.time() - startTime
# cleanup the IO streams and close any open windows
cv2.destroyAllWindows()
vs.stop()
# report time data
if (frames != 0):
    meanTime = np.divide(totalTime ,frames)
else:
    print("no frames found!")
print("Average frame time:", meanTime)
if (meanTime != 0):
    print("Average FPS:", np.divide(1.0, meanTime))
print("Frames:", frames)
print("Frames with a cone:", coneFrames)
if (coneFrames != 0) and (frames != 0):
    print("Frames with a cone (%):", np.divide(coneFrames, frames) * 100)
