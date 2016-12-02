from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
from transform import four_point_transform #this is used for perspective transformation
import serial #this is used for establishing connection between arduino and pi using usb cable

#ser = serial.Serial('/dev/ttyACM0',9600) #Enter the port and baudrate of arduino
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))

#these coordinaes are calculate dta the time of caliberation of camera and will be fixed once the caliberation is done
coord = [(251, 314), (443, 306), (616, 435), (85, 445)]


for frame in camera.capture_continuous(rawCapture, format= "bgr", use_video_port=True):
    image = frame.array
    key = cv2.waitKey(1) & 0xFF    
    rawCapture.truncate(0)
    #to terminate the program just type q
    if key == ord("q"):
        break
    #the center of the image is the center of the trike
    cv2.circle(image,(320,435), 5, (0,0,255), -1)
    #the image is rotated and translated according to the caliberation data 
    warped = four_point_transform(image, np.array(coord))
    #gray scale conversion of image
    gray = cv2.cvtColor(warped, cv2.COLOR_BGR2GRAY) 
    
    #pre-processing of the image
    high_thresh, thresh_im = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    lowThresh = 0.5*high_thresh
    edges = cv2.Canny(gray, 50, 150)
    kernel = np.ones((3,3),np.uint8)
    dilation = cv2.dilate(edges,kernel,iterations = 1)
    im = cv2.bilateralFilter(dilation, 5, 17,17)
    #hough line transformation to find the lane edges    
    lines = cv2.HoughLinesP(im, 1, np.pi/180.0, 50, np.array([]), 100, 10)
    
    #calculation for calculating the distance in cm
    min_x = 640
    max_x = 0
    if lines is not None:
            
            for [x1,y1,x2,y2] in lines[0]:
                    print(x1,x2,y1,y2)
                    #dx, dy = x2 - x1, y2 - y1
                    #angle = np.arctan2(dy, dx)*180/(np.pi) 
                    if abs(y2-215) <= 10 and x2 < min_x and x2>266:
                        min_x = x2
                    if abs(y2-215) <= 10 and x2 > max_x and (x2-266)<0:
                        max_x = x2
                        
                    cv2.line(warped,(x1,y1),(x2,y2),(0,255,0),2)
            position_right = (min_x - 265.0)*(60.0/531.0)*2.54
            position_left = (max_x - 265.0)*(60.0/531.0)*2.54
            #position = '0'*int((min_x - 247.0)*(60.0/523.0)*2.54) #information send to arduino is in this format
            #ser.write(position) #this sends the position data to arduino
            print("distance from right edge is: ",position_right)
            print("distance from left edge is: ",position_left)
    else:
        position_right = 1#14.518546845
        position_left = 114.518546845
        #position = int(114)*'0'
        #ser.write(position)
        print("distance from right edge is: ",position_right)
        print("distance from left edge is: ",position_left)

    
    cv2.imshow('frame', warped)
    print(warped.shape)
