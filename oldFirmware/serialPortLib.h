#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <iostream>
#include <string>
#include <stdexcept>
#include <libserial/SerialPort.h>  // For libserial, assuming you have it installed

class SerialPort {
private:
    SerialPort* serial;
    bool connected;
public:
    SerialPort(const char* portName);
    ~SerialPort();

    int readSerialPort(char* buffer, unsigned int buf_size);
    bool writeSerialPort(char* buffer, unsigned int buf_size);
    bool isConnected();
};

#endif // SERIALPORT_H
