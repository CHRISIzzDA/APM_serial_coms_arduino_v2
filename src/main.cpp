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
const char *delimiter = ";"; //old <>;
char  myString[64];
char pumpData[4];   //old 6
int  iPumpData = 0;
char fanData[4];    //old 6
int iFanData = 0;
const byte numChars = 64;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;
boolean inputRecieved = false;


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

void setNewData() {
    if (newData) {
        strcpy(myString, receivedChars);
        token = strtok(myString, delimiter);

        int i = 0;
        while (token != nullptr) {
            switch (i) {
                case 0:
                    strcpy(pumpData, token);
                    
                    //converting into int
                    iPumpData = atoi(pumpData);
                    analogWrite(pump, iPumpData);
                    Serial.print(iPumpData);
                    break;
                case 1:
                    strcpy(fanData, token);
                     
                    Serial.print(";");

                    //converting into int
                    iFanData = atoi(fanData);
                    
                    Serial.println(iFanData);
                    break;
                default:
                    Serial.println(token);
            }

            i++;
            token = strtok(nullptr, delimiter);
        }
        newData = false;
        inputRecieved = true;
    }
}

void sendData() {
    if (inputRecieved) {
        //sending out Response
        
       
        inputRecieved = false;
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

void loop(){

    recvWithEndMarker();
    setNewData();
    sendData();

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