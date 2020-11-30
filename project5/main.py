from jetbot import Robot
from jetbot import Camera
import traitlets
import cv2

from control import catch_lane_line
from control import find_corner
from control import compute_mid_offset
from control import fix

from numba import jit
robot = Robot()

@jit
def main():
	width = 640
	height = 480
	fps = 21
	capture_width = 3280
	capture_height = 2464

	str = 'nvarguscamerasrc ! video/x-raw(memory:NVMM), width=%d, height=%d, format=(string)NV12, framerate=(fraction)%d/1 ! nvvidconv ! video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! videoconvert ! appsink' % (
				capture_width, capture_height, fps, width, height)

	cap = cv2.VideoCapture(str, cv2.CAP_GSTREAMER)
	
	left_speed, right_speed = 0.28,0.28
	
	while True:
		ret, frame = cap.read()
		if not ret:
			print("Can't receive frame (stream end?). Exiting ...")
			break

		lane_line = catch_lane_line(frame)
		points = find_corner(lane_line)
		mid, offset= compute_mid_offset(points[3],points[2])

		left_speed, right_speed=fix(offset)
		

		robot.set_motors(left_speed, right_speed)

		blue_color = (0, 255, 0)
		cv2.fillPoly(frame, [points], blue_color)
		
		print(left_speed,right_speed)
		cv2.imshow('frame', frame)
		cv2.imshow('lane_line', lane_line)
		if cv2.waitKey(1) >= ord('q'):
			break

	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main()