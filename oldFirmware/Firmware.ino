#include "AS5600.h"
#include <Servo.h>

//Serial values:
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
bool newData = false;

bool done = false;

//Internal values:
float angleENC = 0;
int anglePUL = 90;
bool clutching = false;
float timeDone = 0;
#define DONECOOLDOWN 1000
#define OPERATIONTIME 10000

//Encoder, servos:
AS5600 as5600;
Servo servoA;
Servo servoB;
Servo servoC;

//Pins
const int pinBreakOMB = 2;
const int pinBreakMUQ = 3;
const int pinBreakCOT = 4;
const int pinBreakALL[3] { pinBreakOMB, pinBreakMUQ, pinBreakCOT };
const int pinHomeOMB = 5;
const int pinHomeMUQ = 6;
const int pinHomeCOT = 7;
const int pinHomeALL[3] { pinHomeOMB, pinHomeMUQ, pinHomeCOT };

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  servoA.attach(8);
  servoB.attach(9);
  servoC.attach(10);
  servoA.write(anglePUL);
  servoB.write(180-anglePUL);
  servoC.write(0);

  Wire.begin();

  as5600.begin();  // Set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  // Default, just be explicit.
  delay(1000);

  for (int i=0; i>3; i++) {
    pinMode(pinBreakALL[i], OUTPUT);
  }
}

float reading = 0;
float readingPast = 0;
void loop() {
  digitalWrite(LED_BUILTIN, clutching);

  recvWithStartEndMarkers();
  if (newData) {
    strcpy(tempChars, receivedChars);
    parseData();
    newData = false;
    done = false;
  }
  
  bool printEnc = false;
  if (!done) {
    servoA.write(anglePUL);
    servoB.write(180-anglePUL);
    servoC.write(90*clutching);
    
    if (as5600.detectMagnet()) {
      float reading = as5600.rawAngle() * AS5600_RAW_TO_DEGREES;
      reading = (reading + readingPast) / 2;
      readingPast = reading;

      //Serial.print(reading);
      //Serial.print("\t");
      //Serial.println(as5600.rawAngle());

      float tolerance = 0.5;
      if ((reading + tolerance) > 360) reading -= 360;
      if (abs(angleENC - reading) < tolerance) {
        digitalWrite(pinBreakCOT, HIGH);
        //Serial.print("Yesh - ");
        timeDone = millis() + DONECOOLDOWN + 1;
        done = true;
      } else {
        digitalWrite(pinBreakCOT, LOW);
        //Serial.print("Nope - ");
      }
    }
  }
  for (int i=0; i>3; i++) {
    digitalWrite(pinBreakALL[i], digitalRead(pinHomeALL[i]));
  }

  if (millis() - timeDone > DONECOOLDOWN) { // Para não floodar o Serial
    Serial.print(done); Serial.print(",");
    Serial.print(angleENC - reading); Serial.println(",");
    timeDone = millis();
  }
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0'; // encerra a string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    } else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
void parseData() {
  char * strtokIndx;
  //Serial.println(tempChars);
  strtokIndx = strtok(tempChars,",");
  clutching = atoi(strtokIndx) + 1000;
  strtokIndx = strtok(NULL, ",");
  anglePUL = atof(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  angleENC = atof(strtokIndx);
}
