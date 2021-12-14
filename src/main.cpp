#include <Arduino.h>

#define statusLED 13
#define pump A0
#define throughput A1
#define depth A2
#define pumpEnable 8
#define error1 11
#define error2 10
#define fan1 9
#define fan2 6

char *token;
char *pump_token;
char *depth_token;
char *throughput_token;
char *fan_token;
const char *delimiter = "<>;";
char  mystring[64];
const byte numChars = 64;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;


void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;

    // if (Serial.available() > 0) {
    while (Serial.available() > 0 && !newData) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData) {
        strcpy(mystring, receivedChars);
        token = strtok(mystring, delimiter);

        strcpy(pump_token, token);
        //Serial.println(token);
        token=strtok(nullptr, delimiter);

        strcpy(depth_token, token);
        //Serial.println(token);
        token=strtok(nullptr, delimiter);

        strcpy(throughput_token, token);
        //Serial.println(token);
        token=strtok(nullptr, delimiter);

        strcpy(fan_token,token);
        //Serial.println(token);
        token=strtok(nullptr, delimiter);

        Serial.print("|");
        Serial.print(receivedChars);
        Serial.println("|");
        newData = false;
    }
}

void setup() {
    pinMode(statusLED, OUTPUT);
    pinMode(pump, OUTPUT);
    pinMode(pumpEnable, OUTPUT);
    pinMode(fan1, OUTPUT);
    pinMode(fan2, OUTPUT);
    pinMode(throughput,INPUT);
    pinMode(depth, INPUT);
    pinMode(error1, INPUT);
    pinMode(error2, INPUT);

    Serial.begin(9600);
}

void loop() {

    recvWithEndMarker();
    showNewData();

    //obsolete
/*
    if (receive_buffer == "on")
    {
        digitalWrite(statusLED , HIGH);
    }
    else if (receive_buffer == "off")
    {
        digitalWrite(statusLED, LOW);
    }
*/
}
