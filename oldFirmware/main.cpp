#include "joint.h"
#include "mathematics.h"
#include "pythonfile.h"
#include "SerialPort.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;

static bool doingMove = false;
//Received values:
static bool doneArduino = true;
static bool doneOdrive = true;

void main() {
    #pragma region Serial
    string dataString = "0";
    static char outputSerial[MAX_DATA_LENGTH];
    //vvvvvv~~IMPORTANTE~~vvvvvv
    static const char* port = "\\\\.\\COM9";    //Ver qual entrada é usada!
    //^^^^^^~~IMPORTANTE~~^^^^^^
    static char incomingSerial[MAX_DATA_LENGTH];
    static SerialPort arduino(port);
    #pragma endregion

    char input;
    constexpr int numJoint = 4;
    static float angles[numJoint];
    Point3D rot_axisX(1, 0, 0);
    Point3D rot_axisY(0, 1, 0);
    Point3D rot_axisZ(0, 0, 1);

    Joint jOmbro(0, 0, 0, rot_axisY);
    Joint jMuque(0.4, 0, 0, rot_axisZ);
    Joint jCotovelo(0.3, 0, 0, rot_axisY);
    Joint jPulso(0.2, 0, 0, rot_axisY);
    vector<Joint>joints = { jOmbro, jMuque, jCotovelo , jPulso };

    do {
        /*ifstream outputFile("odriveOutput.txt");
        string myText;
        getline(outputFile, myText);
        if (myText.c_str() == "1") {
            doneOdrive = true;
            outputFile.close();

            ofstream outputFile("odriveOutput.txt");
            outputFile << "0";
        }
        outputFile.close();*/

        if (!doingMove) {
            pythonFile.open("instructions.py");

            if (false) {
                cout << "Angles 0 to 3:\n";
                cin >> angles[0];
                cin >> angles[1];
                cin >> angles[2];
                cin >> angles[3];
            }
            else {
                angles[0] = 20;
                angles[1] = 10;
                angles[2] = 30;
                angles[3] = 15;
            }
            angles[0] *= DEG_TO_RAD;
            angles[1] *= DEG_TO_RAD;
            angles[2] *= DEG_TO_RAD;
            angles[3] *= DEG_TO_RAD;

            for (int i = 0; i < numJoint; i++) {
                Point3D position;
                for (int j = 0; j <= i; j++) {
                    joints[i].Rotate(Quaternion(angles[j], joints[j].rotAxis, false));
                    position.x += joints[j].endPoint.u.x;
                    position.y += joints[j].endPoint.u.y;
                    position.z += joints[j].endPoint.u.z;
                }
                cout << "Junta " << i << ":\n";
                cout << joints[i].endPoint.u.x << "; " << joints[i].endPoint.u.y << "; " << joints[i].endPoint.u.z << "\n";
                cout << position.x << "; " << position.y << "; " << position.z << "\n";
            }
            
            bool isForthAndBack = false;
            cout << "fo(r)th and back / (d)istance / (i)ndex: ";
            cin >> input;
            switch (input) {
            case 'r':
                handleFile(FORTHANDBACK, angles[0], angles[1], angles[2]);
                isForthAndBack = true;
            break;
            case 'd':
                handleFile(TURNS, angles[0], angles[1], angles[2]);
            break;
            case 'i':
                handleFile(INDEFINITE, angles[0], angles[1], angles[2]);
            break;
            default:
            break;
            }
            pythonFile.close();
            doingMove = true;
            doneArduino = false;

            dataString = "<" +
                to_string(!isForthAndBack) + "," +                      // Clutching
                to_string(int(round(angles[3] * RAD_TO_DEG))) + "," +   // Pulse
                to_string(int(round(angles[2] * RAD_TO_DEG))) + ">";    // Encoder
            arduino.writeSerialPort((char*)dataString.c_str(), MAX_DATA_LENGTH);
            cout << "send: " << (char*)dataString.c_str() << endl;
            auto start = high_resolution_clock::now();
            //while (duration_cast<seconds>(high_resolution_clock::now() - start).count() < 5.0)
                //cout << "Waiting...";

            if (isForthAndBack) {
                dataString = "<" +
                    to_string(isForthAndBack) + "," +   // Clutching
                    to_string(90) + "," +               // Pulse
                    to_string(0) + ">";                 // Encoder
                arduino.writeSerialPort((char*)dataString.c_str(), MAX_DATA_LENGTH);
                cout << "send: " << (char*)dataString.c_str() << endl;
                start = high_resolution_clock::now();
                //while (duration_cast<seconds>(high_resolution_clock::now() - start).count() < 5.0)
                    //cout << "Waiting...";
            }


            arduino.readSerialPort(outputSerial, MAX_DATA_LENGTH);
            string outputS = outputSerial;
            doneArduino = (outputS[0] == '1');
            float encoderOffset = stof(outputS.substr(2, outputS.find(",") - 2));
            cout << "\noffset: " << encoderOffset << endl;
            if (doneArduino && doneOdrive) {
                doingMove = false;
            }

            if (encoderOffset > 2) {
                joints[2].Rotate(Quaternion(-angles[2], joints[2].rotAxis, false));
            }
        }

        cout << "Exit? (y/n): ";
        cin >> input;
    } while (input != 'y');
}
