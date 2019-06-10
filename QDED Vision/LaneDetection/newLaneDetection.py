import cv2
import numpy as np
from transform import four_point_transform
import argparse
import matplotlib.image as mpimg
from matplotlib import pyplot as plt
import time

def apply_smoothing(image, kernel_size=15):
    """
    kernel_size must be positive and odd
    """
    return cv2.GaussianBlur(image, (kernel_size, kernel_size), 0)


# What if ROI doesnt contain any lane?
# we can assume that lane is at the rightmost
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help="path to the image file")
args = vars(ap.parse_args())
image = mpimg.imread(args["image"])

im = plt.imread(args["image"])

ax = plt.gca()
fig = plt.gcf()
# implot = ax.imshow(im)
#
# plt.show()
testImg = cv2.imread(args["image"])


# cv2.imshow('before', testImg)

gray = cv2.cvtColor(testImg,cv2.COLOR_BGR2GRAY)
smoothGray = apply_smoothing(gray)

# Good settings for the canny, going to tweak it and see if we can get it a bit better
#imgEdges = cv2.Canny(smoothGray,100, 200)
imgEdges = cv2.Canny(smoothGray,100, 200)
# cv2.circle(imgEdges, (250, 260), 5, (0, 0, 255), -1)
# cv2.circle(imgEdges, (772, 462), 5, (0, 0, 255), -1)
# cv2.circle(imgEdges, (1104, 666), 5, (0, 0, 255), -1)
# cv2.circle(imgEdges, (211, 666), 5, (0, 0, 255), -1)

minLineLength = 100
maxLineGap = 1
smoothLines = cv2.HoughLinesP(imgEdges,1,np.pi/180,15,minLineLength,maxLineGap)
for x in range(0, len(smoothLines)):
    for x1,y1,x2,y2 in smoothLines[x]:
        cv2.line(testImg,(x1,y1),(x2,y2),(0,255,0),2)

# implot = ax.imshow(imgEdges)
#
# plt.show()
# cv2.imshow('after', testImg)
# cv2.imshow('edges', imgEdges)
# cv2.waitKey(0)

cv2.destroyAllWindows()

minLineLength = 10
maxLineGap = 1


# coord = [(241, 316), (438, 312), (602, 447), (54, 447)] DSCN0632
#coord = [(251, 314), (443, 306), (616, 435), (85, 445)]
coord = [(251, 314), (443, 306), (85, 445), (616, 435)]
#imageCoord = [(200, 200), (1200, 0), (1200, 700), (0, 700)]
imageCoord = [(400, 400), (1000, 400), (1100, 700), (200, 700)]
challCoord = [(531, 462), (772, 462), (1104, 666), (211, 666)]


# plt.imshow(lines)
# plt.show()
# print(coord)

# warped = four_point_transform(image, np.array(challCoord))
# plt.imshow(testImg), plt.show()
# # plt.imshow(warped), plt.show()
# cv2.waitKey(0)
# cv2.destroyAllWindows()

cap = cv2.VideoCapture('testvideos/calibration/DSCN0612.mov')

# Get current time to calculate FPS
# Initial frames processed in the beginning == 0

numFrames = 0
coordList = []
numPoints = 0
averageX = 0
averageY = 0
averageX2 = 0
averageY2 = 0

def findRightEdge(angle, x1, y1, x2, y2):
    global averageX, averageY, averageX2, averageY2,numPoints
    # This section finds lines drawn on the right hand side of the video
    # Specifically looks to find an edge in the right hand side of the image.
    if x2 > 320 and y2 > 240:
        # if lineDrawn:
        #     continue
        print("angle = ", angle)
        # lineDrawn = True
        # if(numFrames % fps == 0):
        #     coordList.append([x1,y1])
        coordList.append([x1, y1])
        if numFrames % 50 == 0:
            averageX = 0
            averageY = 0
            numPoints = 0
            averageX2 = 0
            averageY2 = 0
        averageX += x1
        averageX2 += x2
        averageY2 += y2
        averageY += y1
        numPoints += 1
        print("x1 =", x1)
        print("averageX = ", averageX)
        print("averageY =", averageY)


def findLeftEdge():
    print()

def findEdge(angle, x1, y1, x2, y2):
    if angle > 0:
        findRightEdge(angle, x1, y1, x2, y2)
    else:
        findLeftEdge()

while (cap.isOpened()):
    # Get number of frames per second
    fps = cap.get(cv2.CAP_PROP_FPS)


    # print("numFrames", numFrames)
    # print("fps =",fps)
    ret, frame = cap.read()
    if frame is None:
        break

    roi = frame[200:900, 108:900]
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #cv2.imshow('gray', gray)
    high_thresh, thresh_im = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    #cv2.imshow('high thresh',thresh_im)
    #cv2.waitKey(0)
    lowThresh = 0.5 * high_thresh
    edges = cv2.Canny(gray, 100, 200)

    kernel = np.ones((3, 3), np.uint8)
    #kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    dilation = cv2.dilate(edges, kernel, iterations=1)
    im = cv2.bilateralFilter(dilation, 5, 17, 17)
    # cv2.imshow('im',im)
    lines = cv2.HoughLinesP(im, 1, np.pi / 180.0, 50, np.array([]), 100, 15)
    lineDrawn = False
    if lines is not None:
        for [[x1, y1, x2, y2]] in lines:

                #print(x1,x2,y1,y2)
                dx, dy = x2 - x1, y2 - y1
                angle = np.arctan2(dy, dx) * 180 / (np.pi)

                cv2.line(frame, (320, 0), (320, 480), (0, 0, 0), 2)
                findEdge(angle, x1, y1, x2, y2)

                #if angle > 0:

        if numPoints > 0:
            cv2.circle(frame, (int(averageX / numPoints), int(averageY / numPoints)), 5, (0, 0, 255), -1)
            cv2.circle(frame, (int(averageX2 / numPoints), int(averageY2 / numPoints)), 5, (255, 0, 0), -1)
            cv2.line(frame, (int(averageX / numPoints), int(averageY / numPoints)), (int(averageX2 / numPoints), int(averageY2 / numPoints)), (0, 255, 0), 2)
    numFrames += 1
    cv2.imshow('frame', frame)
    # cv2.waitKey(0)

    if cv2.waitKey(1) & 0xFF == ord('q'):#if cv2.waitKey(20) == 27:
       break

cap.release()
cv2.destroyAllWindows()
# averageX = 0
# averageY = 0
# numCoords = 0
# for coord in coordList:
#     print("coords =",coord)
#     averageX += coord[0]
#     averageY += coord[1]
#     numCoords += 1
# print("averageX =", averageX / numCoords)
# print("averageY =", averageY / numCoords)