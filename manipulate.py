import pynpput.keyboard import key, Listener
from jetbot import Robot

robot = Robot() 

def stop():
	robot.stop()

def forward():
	robot.forward(0.5)

def turn_left():
	robot.left(0.5)

def turn_right():
	robot.right(0.5)

def backward():
	robot.backward(0.5)

def on_press(key):
	k = str(key).replace("'","")
	if(k=="w"):
		forward()
	elif (k =="a"):
		turn_left()
	elif (k =="s"):
		backward()
	elif (k=="d"):
		turn_right()

def on_release(key):
	stop()

with Listener(on_press=on_press, on_release=on_release) as listener:
	listener.join()
