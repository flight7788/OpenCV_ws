import cv2
import numpy as np
from numba import jit

@jit
def catch_lane_line(src):
	
	height, width = src.shape[:2]
	
	half = src
	half[0:int(height/2), 0:width] = (0,0,0)

	cv2.cvtColor(src, cv2.COLOR_BGR2GRAY )


	hsv = cv2.cvtColor(half, cv2.COLOR_BGR2HSV)

	lower_white = np.array([0, 0, 200])
	upper_white = np.array([200, 255, 255])
	mask = cv2.inRange(hsv, lower_white, upper_white)

	white = cv2.bitwise_and(hsv, hsv, mask = mask)

	rgb = cv2.cvtColor(white, cv2.COLOR_HSV2RGB)
	gray = cv2.cvtColor(rgb, cv2. COLOR_BGR2GRAY)
	
	eq_hist = cv2.equalizeHist(gray)

	ret, binary = cv2.threshold(eq_hist, 20, 255, cv2.THRESH_BINARY) 
	kernel = np.ones((5,5), np.uint8)
	opening = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations = 1)
	closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel, iterations = 2)
	return closing

@jit
def find_corner(src):
	height, width = src.shape[:2]
	half_height = int(height/2)
	half_width = int(width / 2)
	p0_x = 0
	p0_y = height - 1
	p1_x = width - 1
	p1_y = height - 1
	p2_x = width - 1
	p2_y = half_height
	p3_x = 0
	p3_y = 0

	flag = False
	for y in range(height - 1, half_height, -1):
		for x in range(0, half_width):
			if(src[y,x] >= 255):
				p0_x = x
				p0_y = y
				flag = True
				break
		if(flag):
			break

	flag = False
	for y in range(height - 1, half_height, -1):
		for x in range(width - 1, half_width, -1):
			if(src[y,x] >= 255):
				p1_x = x
				p1_y = y
				flag = True
				break
		if(flag):
			break

	flag = False
	for y in range(half_height, height):
		for x in range(width - 1, half_width, -1):
			if(src[y,x] >= 255):
				p2_x = x
				p2_y = y
				flag = True
				break
		if(flag):
			break

	flag = False
	for y in range(half_height, height):
		for x in range(0, half_width):
			if(src[y,x] >= 255):
				p3_x = x
				p3_y = y
				flag = True
				break
		if(flag):
			break

	points = np.array([[p0_x, p0_y], [p1_x, p1_y], [p2_x, p2_y], [p3_x, p3_y]])	
	return points

@jit
def compute_mid_offset(pt1,pt2):
	mid = (pt1[0]+pt2[0])/2
	return mid, mid-390

@jit
def fix(offset):
	left_speed = 0.3
	right_speed = 0.3
	if(offset>30):
		diff = (offset-30)/30*0.04
		left_speed = left_speed+0.04 if diff > 0.04 else left_speed + diff
	if(offset<-30):
		offset =abs(offset)
		diff = (offset-30)/30*0.05
		right_speed = right_speed + 0.05 if diff > 0.05 else right_speed + diff
	return left_speed, right_speed

if __name__ == '__main__':
	cap = cv2.VideoCapture('output.avi')

	while cap.isOpened():
		ret, frame = cap.read()
		if not ret:
			print("Can't receive frame (stream end?). Exiting ...")
			break

		lane_line = catch_lane_line(frame)
		points = find_corner(lane_line)
		slope = compute_slope(points[0], points[1])
		print(slope)
		car_dir = judge_car_dir(slope)
		left_speed, right_speed = regulate(car_dir, slope)
		print(left_speed," ",right_speed)


		blue_color = (0, 255, 0)
		cv2.fillPoly(frame, [points], blue_color)


		cv2.imshow('frame', frame)
		cv2.imshow('lane_line', lane_line)
		if cv2.waitKey(1) == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()