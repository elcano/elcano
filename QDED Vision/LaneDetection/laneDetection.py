import cv2
import numpy as np 
from transform import four_point_transform
import argparse
import matplotlib.image as mpimg
from matplotlib import pyplot as plt
# What if ROI doesnt contain any lane?
# we can assume that lane is at the rightmost 
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image file")
args = vars(ap.parse_args())
image = mpimg.imread(args["image"])

im = plt.imread(args["image"])
#taking input from user
ax = plt.gca()
fig = plt.gcf()
implot = ax.imshow(im)
# coord = []
# coord = [(241, 316), (438, 312), (602, 447), (54, 447)] DSCN0632
coord = [(251, 314), (443, 306), (616, 435), (85, 445)]
# def onclick(event):
# 	if event.xdata != None and event.ydata != None :
# 		coord.append((int(event.xdata), int(event.ydata)))
# cid = fig.canvas.mpl_connect('button_press_event', onclick)
# plt.show()
# print(coord)

warped = four_point_transform(image, np.array(coord))
plt.imshow(warped),plt.show()
# cv2.waitKey(0)
cv2.destroyAllWindows()

cap = cv2.VideoCapture('testvideos/calibration/DSCN0622.MOV')

while (cap.isOpened()):
	ret, frame = cap.read()
	if frame is None:
		break
	# roi = frame[250:480, 0:640]
	cv2.circle(frame,(320,435), 5, (0,0,255), -1)
	cv2.imshow("Input",frame);	
	warped = four_point_transform(frame, np.array(coord))
	gray = cv2.cvtColor(warped, cv2.COLOR_BGR2GRAY)
	
	#houghline transform
	high_thresh, thresh_im = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
	lowThresh = 0.5*high_thresh
	edges = cv2.Canny(gray, 50, 150)
	kernel = np.ones((3,3),np.uint8)
	dilation = cv2.dilate(edges,kernel,iterations = 1)
	im = cv2.bilateralFilter(dilation, 5, 17,17)
	#cv2.imshow('im',im)
	lines = cv2.HoughLinesP(im, 1, np.pi/180.0, 50, np.array([]), 100, 10)
	
	min_x = 640
	if lines is not None:
		for [[x1,y1,x2,y2]] in lines:		
			dx, dy = x2 - x1, y2 - y1
			angle = np.arctan2(dy, dx)*180/(np.pi)
			# print(angle)

			if angle >0:
				# print(x2, y2)	
				if abs(y2-210) <= 10 and x2 < min_x:
					# print("yes")
					min_x = x2

				cv2.line(warped,(x1,y1),(x2,y2),(0,255,0),2)
		print((min_x - 247.0)*(60.0/523.0))
	# font = cv2.FONT_HERSHEY_SIMPLEX
	# cv2.putText(frame,'Hello World!',(10,500), font, 1,(255,255,255),2)
	cv2.imshow('frame', warped)
	#cv2.waitKey(0)
			# cv2.waitKey(0)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

# stictching of closer line can be done after hough transform.

cap.release()
cv2.destroyAllWindows()
