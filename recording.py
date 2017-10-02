#recording.py
import picamera
from picamera.array import PiRGBArray
from time import sleep
import os.path


videoNum = 0
pathExists = os.path.exists("/home/pi/video" + str(videoNum) + ".h264")
camera = picamera.PiCamera()
camera.resolution = (640,480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640,480))

while pathExists:
    videoNum += 1
    pathExists = os.path.exists("/home/pi/video" + str(videoNum) + ".h264")


camera.start_preview()
camera.start_recording("/home/pi/video" + str(videoNum) + ".h264")
camera.wait_recording(20)
camera.stop_recording()
camera.stop_preview()