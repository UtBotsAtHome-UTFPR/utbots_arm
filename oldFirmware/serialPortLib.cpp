#include "serialPortLib.h"

SerialPort::SerialPort(const char* portName) {
    this->connected = false;

    try {
        this->serial = new SerialPort(portName);  // Open the serial port
        
        // Set serial port parameters
        serial->setBaudRate(SerialPort::BAUD_9600);
        serial->setTimeout(SerialPort::TIMEOUT_BLOCKING);
        serial->setDataBits(SerialPort::DATA_BITS_8);
        serial->setParity(SerialPort::PARITY_NONE);
        serial->setStopBits(SerialPort::STOP_BITS_1);

        this->connected = true;  // Connection established

        // Sleep for a short time to ensure connection is established
        std::this_thread::sleep_for(std::chrono::milliseconds(ARDUINO_WAIT_TIME));
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: Could not open serial port " << portName << ": " << e.what() << std::endl;
    }
}

SerialPort::~SerialPort()
{
    if (this->connected) {
        this->connected = false;
        delete this->serial;  // Close the serial port
    }
}

int SerialPort::readSerialPort(char* buffer, unsigned int buf_size) {
    if (!this->connected) return 0;

    try {
        int bytesRead = serial->readBytes(buffer, buf_size);
        return bytesRead;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR reading from serial port: " << e.what() << std::endl;
        return 0;
    }
}

bool SerialPort::writeSerialPort(char* buffer, unsigned int buf_size)
{
    if (!this->connected) return false;

    try {
        serial->writeBytes(buffer, buf_size);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR writing to serial port: " << e.what() << std::endl;
        return false;
    }
}

bool SerialPort::isConnected()
{
    return this->connected;
}
