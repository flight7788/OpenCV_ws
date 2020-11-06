from jetbot import Robot
from jetbot import Camera
import traitlets
import cv2

robot = Robot()

width = 224
height = 224
fps = 21
capture_width = 3280
capture_height = 2464

str = 'nvarguscamerasrc ! video/x-raw(memory:NVMM), width=%d, height=%d, format=(string)NV12, framerate=(fraction)%d/1 ! nvvidconv ! video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! videoconvert ! appsink' % (
                capture_width, capture_height, fps, width, height)

cap = cv2.VideoCapture(str, cv2.CAP_GSTREAMER)

while(True):
    re, image = cap.read()



    
    left, right = 0
    robot.set_motors(left, right)
    cv2.imshow("frame",image)

    if(cv2.waitKey(1) == ord('q')):
        break
