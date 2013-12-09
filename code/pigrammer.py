#!/usr/bin/python

import RPi.GPIO as GPIO
import subprocess
import argparse
import time

GPIO_GREENLED = 4
GPIO_YELLOWLED = 17
GPIO_REDLED = 27
GPIO_BLUELED = 22
GPIO_BUTTON = 23

def setupIO():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(GPIO_GREENLED, GPIO.OUT)
    GPIO.output(GPIO_GREENLED, 0)
    GPIO.setup(GPIO_YELLOWLED, GPIO.OUT)
    GPIO.output(GPIO_YELLOWLED, 0)
    GPIO.setup(GPIO_REDLED, GPIO.OUT)
    GPIO.output(GPIO_REDLED, 0)
    GPIO.setup(GPIO_BLUELED, GPIO.OUT)
    GPIO.output(GPIO_BLUELED, 0)
    GPIO.setup(GPIO_BUTTON, GPIO.IN)

def setBlueLED(ledState):
    GPIO.output(GPIO_BLUELED, ledState)

def setRedLED(ledState):
    GPIO.output(GPIO_REDLED, ledState)

def setYellowLED(ledState):
    GPIO.output(GPIO_YELLOWLED, ledState)

def setGreenLED(ledState):
    GPIO.output(GPIO_GREENLED, ledState)

def waitForButton():
    while GPIO.input(GPIO_BUTTON) != 0:
        pass
    time.sleep(0.05)
    while GPIO.input(GPIO_BUTTON) != 1:
        pass
    time.sleep(0.05)
    while GPIO.input(GPIO_BUTTON) != 0:
        pass
    time.sleep(0.05)

parser = argparse.ArgumentParser(description='Batch programmer for AVR using Raspberry Pi and Makefile')
parser.add_argument('rule', help='Name of Makefile rule to flash code onto device')
parser.add_argument('num_devices', type=int, help='Number of devices to program before terminating')

args = parser.parse_args()

numTarget = args.num_devices
numSuccess = 0
numFail = 0

makeRule = args.rule

setupIO()

setBlueLED(0)
setRedLED(0)
setYellowLED(0)
setGreenLED(1)

while numSuccess < numTarget:
    print ">>>%d successful of %d, %d failed. Ready to program...\n" % (numSuccess, numTarget, numFail)
    
    waitForButton()

    setBlueLED(0)
    setRedLED(0)
    setYellowLED(1)
    setGreenLED(0)

    if subprocess.call(["make", makeRule, "BATCH_MODE=pigrammer"]) == 0:
        if subprocess.call(["make", "fuse", "BATCH_MODE=pigrammer"]) == 0:
            setBlueLED(0)
            setRedLED(0)
            setYellowLED(0)
            setGreenLED(1)
            numSuccess = numSuccess + 1
            print "Success!\n"
        else:
            setBlueLED(0)
            setRedLED(1)
            setYellowLED(0)
            setGreenLED(0)
            numFail = numFail + 1
            print "Failed!\n"
    else:
        setBlueLED(0)
        setRedLED(1)
        setYellowLED(0)
        setGreenLED(0)
        numFail = numFail + 1
        print "Failed!\n"

setBlueLED(1)
setRedLED(0)
setYellowLED(0)
setGreenLED(0)

input("Programming run completed. Press Enter to continue...")
exit()
