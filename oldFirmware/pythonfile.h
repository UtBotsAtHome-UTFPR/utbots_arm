#pragma once
#include "mathematics.h"
#include <fstream>
#include <iostream>
using namespace std;
#define ROXO true
#define VERM true

enum RotMode {
    FORTHANDBACK, INDEFINITE, TURNS
};

enum FinishMode {
    DISTANCE, INDEX, VELOCITY
};

ofstream pythonFile;

string interpretCode(int code) {
    switch (code) {
    case 0:
        return "odrive0.axis0";
    break;
    case 1:
        return "odrive0.axis1";
    break;
    case 2:
        return "odrive1.axis0";
    break;
    case 3:
        return "odrive1.axis1";
    break;
    default:
        return "";
    break;
    }
}

void finishOnWhat(FinishMode criteria) {
    pythonFile << "\n";
    if (ROXO) { pythonFile << interpretCode(0) + ".config.sensorless_ramp.finish_on_distance = "; pythonFile << (criteria == DISTANCE); pythonFile << "\n"; }
    if (ROXO) { pythonFile << interpretCode(0) + ".config.sensorless_ramp.finish_on_enc_idx = "; pythonFile << (criteria == INDEX); pythonFile << "\n"; }
    if (ROXO) { pythonFile << interpretCode(0) + ".config.sensorless_ramp.finish_on_vel = "; pythonFile << (criteria == VELOCITY); pythonFile << "\n"; }
    if (ROXO) { pythonFile << interpretCode(1) + ".config.sensorless_ramp.finish_on_distance = "; pythonFile << (1); pythonFile << "\n"; }
    if (ROXO) { pythonFile << interpretCode(1) + ".config.sensorless_ramp.finish_on_enc_idx = "; pythonFile << (0); pythonFile << "\n"; }
    if (ROXO) { pythonFile << interpretCode(1) + ".config.sensorless_ramp.finish_on_vel = "; pythonFile << (0); pythonFile << "\n"; }
    if (VERM) { pythonFile << interpretCode(2) + ".config.sensorless_ramp.finish_on_distance = "; pythonFile << (criteria == DISTANCE); pythonFile << "\n"; }
    if (VERM) { pythonFile << interpretCode(2) + ".config.sensorless_ramp.finish_on_enc_idx = "; pythonFile << (criteria == INDEX); pythonFile << "\n"; }
    if (VERM) { pythonFile << interpretCode(2) + ".config.sensorless_ramp.finish_on_vel = "; pythonFile << (criteria == VELOCITY); pythonFile << "\n"; }
    pythonFile << "\n";
}

void stopOrGo(bool go, int code, string indent) {
    pythonFile << indent; pythonFile << interpretCode(code) + ".config.sensorless_ramp.accel = "; pythonFile << (go ? 25 : 0); pythonFile << "\n";
    pythonFile << indent; pythonFile << interpretCode(code) + ".config.sensorless_ramp.vel = "; pythonFile << (go ? 150 : 0); pythonFile << "\n";
}

void clearErrors() {
    if (ROXO) { pythonFile << interpretCode(0) + ".clear_errors()\n"; }
    if (ROXO) { pythonFile << interpretCode(1) + ".clear_errors()\n"; }
    if (VERM) { pythonFile << interpretCode(2) + ".clear_errors()\n"; }
}

void handleFile(RotMode mode, float odrive1axis0, float odrive0axis1, float odrive0axis0) {
    pythonFile << "from __future__ import print_function\n\n";
    pythonFile << "import odrive\n";
    pythonFile << "from odrive.enums import* \n";
    pythonFile << "import time\n";
    pythonFile << "import math\n\n";
    pythonFile << "# Find a connected ODrive(this will block until you connect one)\n";
    pythonFile << "print(\"Finding an odrive...\")\n";
    //pythonFile << "odrive0 = odrive.find_any()\n\n";
    if (ROXO) { pythonFile << "odrive0 = odrive.find_any(serial_number=\"367231693333\")\n"; }
    if (VERM) { pythonFile << "odrive1 = odrive.find_any(serial_number=\"3679315C3333\")\n\n"; }

    if (ROXO) { pythonFile << interpretCode(0) + ".config.sensorless_ramp.finish_distance = "; pythonFile << 50 * 20 * odrive0axis0; pythonFile << "\n"; }
    if (ROXO) { pythonFile << interpretCode(1) + ".config.sensorless_ramp.finish_distance = "; pythonFile << 50 * 21 * odrive0axis1; pythonFile << "\n"; }
    if (VERM) { pythonFile << interpretCode(2) + ".config.sensorless_ramp.finish_distance = "; pythonFile << 50 * 15 * odrive1axis0; pythonFile << "\n"; }
    switch (mode) {
    case FORTHANDBACK:
    //break;
    case TURNS:
        finishOnWhat(DISTANCE);
    break;
    case INDEFINITE:
        finishOnWhat(INDEX);
    break;
    default:
    break;
    }

    clearErrors();
    bool repetition = true;
    do {
        pythonFile << "\n"; if (ROXO)stopOrGo(1, 0, ""); if (ROXO)stopOrGo(1, 1, ""); if (VERM)stopOrGo(1, 2, "");
        //pythonFile << interpretCode(2) + ".requested_state = AXIS_STATE_SENSORLESS_CONTROL\n";
        for (int i = (!ROXO ? 2 : 0); i < (!VERM ? 2 : 3); i++) {
            pythonFile << interpretCode(i) + ".requested_state = AXIS_STATE_SENSORLESS_CONTROL\n";
            pythonFile << "while "; pythonFile << interpretCode(i) + ".motor.current_control.Iq_setpoint and ";
            pythonFile << interpretCode(i) + ".config.sensorless_ramp.accel:\n";
            pythonFile << "    print(\"Moving...\")\n";
            pythonFile << "    if "; pythonFile << interpretCode(i) + ".error != 0:\n";
            pythonFile << "        time.sleep(0.25)\n";
            pythonFile << "        "; pythonFile << interpretCode(i) + ".clear_errors()\n";
            pythonFile << "        time.sleep(0.25)\n";
            pythonFile << "    if "; pythonFile << interpretCode(i) + ".motor.current_control.Iq_setpoint == 0:\n";
            stopOrGo(0, i, "        ");
            pythonFile << "        "; pythonFile << interpretCode(i) + ".requested_state = AXIS_STATE_SENSORLESS_CONTROL\n\n";
        }
        if (mode == FORTHANDBACK) {
            if (ROXO) { pythonFile << interpretCode(0) + ".motor.config.direction *= -1\n"; }
            if (ROXO) { pythonFile << interpretCode(1) + ".motor.config.direction *= -1\n"; }
            if (VERM) { pythonFile << interpretCode(2) + ".motor.config.direction *= -1\n"; }
            mode = TURNS;	// Anything but FORTHANDBACK
        } else {
            repetition = false;
        }
        //stopOrGo(0, 0); stopOrGo(0, 1); stopOrGo(0, 2);
    } while (repetition);
    
    if (mode == FORTHANDBACK) {
        if (ROXO) { pythonFile << interpretCode(0) + ".motor.config.direction *= -1\n"; }
        if (ROXO) { pythonFile << interpretCode(1) + ".motor.config.direction *= -1\n"; }
        if (VERM) { pythonFile << interpretCode(2) + ".motor.config.direction *= -1\n"; }
    }
    pythonFile << "\nprint(\"Bus voltage is \" + str(odrive0.vbus_voltage) + \"V\")\n";

    //pythonFile << "\n\nf = open(\"odriveOutput.txt\", \"w\")";
    //pythonFile << "\nf.write(\"1\")";
    //pythonFile << "\nf.close()";
}
