#include <Arduino.h>

String receive_buffer;

void setup() {
    pinMode(13, OUTPUT);
    pinMode(A0, OUTPUT);
    Serial.begin(9600);
}

void loop() {

    if(Serial.available()>0)
    {
        receive_buffer = Serial.readStringUntil('\n');

        Serial.print("Read |");
        Serial.print(receive_buffer);
        Serial.println("|");
    }

    if (receive_buffer == "on")
    {
        digitalWrite(13 , HIGH);
    }
    else if (receive_buffer == "off")
    {
        digitalWrite(13, LOW);
    }
}