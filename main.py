from jetbot import Robot
from jetbot import Camera
import traitlets
import cv2
import control

robot = Robot()



if __name__ == '__main__':
	width = 224
	height = 224
	fps = 21
	capture_width = 3280
	capture_height = 2464

	str = 'nvarguscamerasrc ! video/x-raw(memory:NVMM), width=%d, height=%d, format=(string)NV12, framerate=(fraction)%d/1 ! nvvidconv ! video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! videoconvert ! appsink' % (
				capture_width, capture_height, fps, width, height)

	cap = cv2.VideoCapture(str, cv2.CAP_GSTREAMER)

	while cap.isOpened():
		ret, frame = cap.read()
		frame = cv2.resize(frame,(640,480),interpolation=cv2.INTER_CUBIC)
		if not ret:
			print("Can't receive frame (stream end?). Exiting ...")
			break

		lane_line = control.catch_lane_line(frame)
		points = control.find_corner(lane_line)
		slope = control.compute_slope(points[0], points[1])
		print(slope)
		car_dir = control.judge_car_dir(slope)
		left_speed, right_speed = control.regulate(car_dir, slope)
		print(left_speed," ",right_speed)

		robot.set_motors(left_speed, right_speed)
		blue_color = (0, 255, 0)
		cv2.fillPoly(frame, [points], blue_color)


		cv2.imshow('frame', frame)
		cv2.imshow('lane_line', lane_line)
		if cv2.waitKey(1) == ord('q'):
			break

	cap.release()
	cv2.destroyAllWindows()