#!/usr/bin/python3
import Adafruit_DHT
import RPi.GPIO as GPIO
import sys
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

tem = []
hum = []

segments = (21,17,14,12,11,20,15,13)

for segment in segments:
    GPIO.setup(segment, GPIO.OUT)
    GPIO.output(segment, 1)

digits = (16, 18, 19, 22)

for digit in digits:
    GPIO.setup(digit, GPIO.OUT)
    GPIO.output(digit, 0)

num = [(0,0,0,0,0,0,1),
       (1,0,0,1,1,1,1),
       (0,0,1,0,0,1,0),
       (0,0,0,0,1,1,0),
       (1,0,0,1,1,0,0),
       (0,1,0,0,1,0,0),
       (0,1,0,0,0,0,0),
       (0,0,0,1,1,1,1),
       (0,0,0,0,0,0,0),
       (0,0,0,0,1,0,0)]

units = {'C':(0,1,1,0,0,0,1),
         'P':(0,0,1,1,0,0,0)}

def write_digit(d, n):
    for i in range(7):
        GPIO.output(segments[i], num[n][i])
    GPIO.output(digits[d], 1)
    time.sleep(0.001)
    GPIO.output(digits[d], 0)
    time.sleep(0.009)

def write_unit(d, u):
    for i in range(7):
        GPIO.output(segments[i], units[u][i])
    GPIO.output(digits[d], 1)
    time.sleep(0.001)
    GPIO.output(digits[d], 0)
    time.sleep(0.009)

def write_number(n, u):
    for j in range(200):
        p = n
        write_unit(0, u);
        for i in range(1, 4):
            d = p % 10
            write_digit(i, d)
            p //= 10
            if p == 0: break

try:
  while True:
    h, t= Adafruit_DHT.read_retry(11, 4)

    tem.append(t)
    temlen = len(tem)
    if temlen > 10:
        tem = tem[1:]
        temlen = 10
    hum.append(h)
    humlen = len(hum)
    if humlen > 10:
        hum = hum[1:]
        humlen = 10

    write_number(int(sum(tem)/temlen), 'C')
    write_number(int(sum(hum)/humlen), 'P')
finally:
  GPIO.cleanup()
  
