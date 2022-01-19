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

typedef struct
{
    uint16_t _depth;
    uint16_t _flow;
}Pair_16_t;
//----------------------------------------------------------------------------------------------------------------------


// Variables
const byte numChars = 64;
char receivedChars[numChars]; // an array to store the received data
char  myString[numChars];
char *token;
const char *delimiter = ";";

char pumpData[4];
char* endPtrPump;
int  iPumpData = 0;

char fanData[4];
char* endPtrFan;
int iFanData = 0;

//all Depth values are calculated by depth in m / 0.05859375

Pair_16_t  dataLvl1[5] = {{512,0},
                          {529,0},
                          {495,0},
                          {563,0},
                          {546,0}};
int dataCounterLvl1 = 0;

Pair_16_t dataLvl2[5] = {{495, 15},
                         {461, 15},
                         {427, 15},
                         {444, 15},
                         {427, 15}};
int dataCounterLvl2 = 0;

Pair_16_t dataLvl3[5] = {{393, 20},
                         {375, 20},
                         {341, 20},
                         {324, 20},
                         {341, 20}};
int dataCounterLvl3 = 0;

Pair_16_t dataLvl4[5] = {{307, 25},
                         {273, 25},
                         {239, 25},
                         {221, 25},
                         {256, 25}};
int dataCounterLvl4 = 0;

Pair_16_t dataLvl5[5] = {{324, 8},
                        {393, 3},
                        {444, 0},
                        {495, 0},
                        {512, 0}};
int dataCounterLvl5 = 0;

boolean newData = false;
boolean inputReceived = false;
//----------------------------------------------------------------------------------------------------------------------


//Functions
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

//------------------------------------------------------------------------
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
                    iPumpData = strtol(pumpData, &endPtrPump, 10);

                    analogWrite(pump, iPumpData);
                    break;
                case 1:
                    strcpy(fanData, token);
                    //converting into int
                    iFanData = strtol(fanData, &endPtrFan, 10);

                    analogWrite(fan1, iFanData);
                    analogWrite(fan2, iFanData);
                    break;
                default:
                    Serial.println(token);
            }

            i++;
            token = strtok(nullptr, delimiter);
        }
        newData = false;
        inputReceived = true;
    }
}

//----------------------------------------------------------------------
void sendData() {
    if (inputReceived) {

        switch (iPumpData) {
            case 0:
                if (iFanData == 0) {
                    Serial.print(dataLvl1[dataCounterLvl1]._depth);
                    Serial.print(";");
                    Serial.println(dataLvl1[dataCounterLvl1]._flow);
                    if (dataCounterLvl1 < (sizeof(dataLvl1) / sizeof(Pair_16_t) )- 1) {
                        ++dataCounterLvl1;
                    }
                }else {
                    Serial.print(dataLvl5[dataCounterLvl5]._depth);
                    Serial.print(";");
                    Serial.println(dataLvl5[dataCounterLvl5]._flow);
                    if (dataCounterLvl5 < (sizeof(dataLvl5) / sizeof(Pair_16_t)) - 1) {
                        ++dataCounterLvl5;
                    }
                }
                break;
            case 100:
                Serial.print(dataLvl2[dataCounterLvl2]._depth);
                Serial.print(";");
                Serial.println(dataLvl2[dataCounterLvl2]._flow);
                if (dataCounterLvl2 < (sizeof(dataLvl2) / sizeof(Pair_16_t) )- 1) {
                    ++dataCounterLvl2;
                }
                break;
            case 500:
                Serial.print(dataLvl3[dataCounterLvl3]._depth);
                Serial.print(";");
                Serial.println(dataLvl3[dataCounterLvl3]._flow);
                if (dataCounterLvl3 < (sizeof(dataLvl3) / sizeof(Pair_16_t) )- 1) {
                    ++dataCounterLvl3;
                }
                break;
            case 1000:
                Serial.print(dataLvl4[dataCounterLvl4]._depth);
                Serial.print(";");
                Serial.println(dataLvl4[dataCounterLvl4]._flow);
                if (dataCounterLvl4 < (sizeof(dataLvl4) / sizeof(Pair_16_t) )- 1) {
                    ++dataCounterLvl4;
                }
                break;
            default:
                Serial.print(iPumpData);
                Serial.print(";");
                Serial.println(iFanData);
                break;
        }

        inputReceived = false;
    }
}
//----------------------------------------------------------------------------------------------------------------------

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

}