from __future__ import print_function

import odrive
from odrive.enums import* 
import time
import math

# Find a connected ODrive(this will block until you connect one)
print("Finding an odrive...")
odrive0 = odrive.find_any(serial_number="367231693333")
odrive1 = odrive.find_any(serial_number="3679315C3333")

odrive0.axis0.config.sensorless_ramp.finish_distance = 523.599
odrive0.axis1.config.sensorless_ramp.finish_distance = 183.26
odrive1.axis0.config.sensorless_ramp.finish_distance = 261.799

odrive0.axis0.config.sensorless_ramp.finish_on_distance = 1
odrive0.axis0.config.sensorless_ramp.finish_on_enc_idx = 0
odrive0.axis0.config.sensorless_ramp.finish_on_vel = 0
odrive0.axis1.config.sensorless_ramp.finish_on_distance = 1
odrive0.axis1.config.sensorless_ramp.finish_on_enc_idx = 0
odrive0.axis1.config.sensorless_ramp.finish_on_vel = 0
odrive1.axis0.config.sensorless_ramp.finish_on_distance = 1
odrive1.axis0.config.sensorless_ramp.finish_on_enc_idx = 0
odrive1.axis0.config.sensorless_ramp.finish_on_vel = 0

odrive0.axis0.clear_errors()
odrive0.axis1.clear_errors()
odrive1.axis0.clear_errors()

odrive0.axis0.config.sensorless_ramp.accel = 25
odrive0.axis0.config.sensorless_ramp.vel = 150
odrive0.axis1.config.sensorless_ramp.accel = 25
odrive0.axis1.config.sensorless_ramp.vel = 150
odrive1.axis0.config.sensorless_ramp.accel = 25
odrive1.axis0.config.sensorless_ramp.vel = 150
odrive0.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL
while odrive0.axis0.motor.current_control.Iq_setpoint and odrive0.axis0.config.sensorless_ramp.accel:
    print("Moving...")
    if odrive0.axis0.error != 0:
        time.sleep(0.25)
        odrive0.axis0.clear_errors()
        time.sleep(0.25)
    if odrive0.axis0.motor.current_control.Iq_setpoint == 0:
        odrive0.axis0.config.sensorless_ramp.accel = 0
        odrive0.axis0.config.sensorless_ramp.vel = 0
        odrive0.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL

odrive0.axis1.requested_state = AXIS_STATE_SENSORLESS_CONTROL
while odrive0.axis1.motor.current_control.Iq_setpoint and odrive0.axis1.config.sensorless_ramp.accel:
    print("Moving...")
    if odrive0.axis1.error != 0:
        time.sleep(0.25)
        odrive0.axis1.clear_errors()
        time.sleep(0.25)
    if odrive0.axis1.motor.current_control.Iq_setpoint == 0:
        odrive0.axis1.config.sensorless_ramp.accel = 0
        odrive0.axis1.config.sensorless_ramp.vel = 0
        odrive0.axis1.requested_state = AXIS_STATE_SENSORLESS_CONTROL

odrive1.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL
while odrive1.axis0.motor.current_control.Iq_setpoint and odrive1.axis0.config.sensorless_ramp.accel:
    print("Moving...")
    if odrive1.axis0.error != 0:
        time.sleep(0.25)
        odrive1.axis0.clear_errors()
        time.sleep(0.25)
    if odrive1.axis0.motor.current_control.Iq_setpoint == 0:
        odrive1.axis0.config.sensorless_ramp.accel = 0
        odrive1.axis0.config.sensorless_ramp.vel = 0
        odrive1.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL

odrive0.axis0.motor.config.direction *= -1
odrive0.axis1.motor.config.direction *= -1
odrive1.axis0.motor.config.direction *= -1

odrive0.axis0.config.sensorless_ramp.accel = 25
odrive0.axis0.config.sensorless_ramp.vel = 150
odrive0.axis1.config.sensorless_ramp.accel = 25
odrive0.axis1.config.sensorless_ramp.vel = 150
odrive1.axis0.config.sensorless_ramp.accel = 25
odrive1.axis0.config.sensorless_ramp.vel = 150
odrive0.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL
while odrive0.axis0.motor.current_control.Iq_setpoint and odrive0.axis0.config.sensorless_ramp.accel:
    print("Moving...")
    if odrive0.axis0.error != 0:
        time.sleep(0.25)
        odrive0.axis0.clear_errors()
        time.sleep(0.25)
    if odrive0.axis0.motor.current_control.Iq_setpoint == 0:
        odrive0.axis0.config.sensorless_ramp.accel = 0
        odrive0.axis0.config.sensorless_ramp.vel = 0
        odrive0.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL

odrive0.axis1.requested_state = AXIS_STATE_SENSORLESS_CONTROL
while odrive0.axis1.motor.current_control.Iq_setpoint and odrive0.axis1.config.sensorless_ramp.accel:
    print("Moving...")
    if odrive0.axis1.error != 0:
        time.sleep(0.25)
        odrive0.axis1.clear_errors()
        time.sleep(0.25)
    if odrive0.axis1.motor.current_control.Iq_setpoint == 0:
        odrive0.axis1.config.sensorless_ramp.accel = 0
        odrive0.axis1.config.sensorless_ramp.vel = 0
        odrive0.axis1.requested_state = AXIS_STATE_SENSORLESS_CONTROL

odrive1.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL
while odrive1.axis0.motor.current_control.Iq_setpoint and odrive1.axis0.config.sensorless_ramp.accel:
    print("Moving...")
    if odrive1.axis0.error != 0:
        time.sleep(0.25)
        odrive1.axis0.clear_errors()
        time.sleep(0.25)
    if odrive1.axis0.motor.current_control.Iq_setpoint == 0:
        odrive1.axis0.config.sensorless_ramp.accel = 0
        odrive1.axis0.config.sensorless_ramp.vel = 0
        odrive1.axis0.requested_state = AXIS_STATE_SENSORLESS_CONTROL


print("Bus voltage is " + str(odrive0.vbus_voltage) + "V")
