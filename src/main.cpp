#include <Arduino.h>

#define StatusLED 13

String receive_buffer;

void setup() {
    pinMode(StatusLED, OUTPUT);
    pinMode(A0, OUTPUT);
    Serial.begin(9600);
}

void loop() {

    if(Serial.available()>0)
    {
        receive_buffer = Serial.readStringUntil('\n');

        Serial.print("Read: |");
        Serial.print(receive_buffer);
        Serial.println("|");
    }

    if (receive_buffer == "on")
    {
        digitalWrite(StatusLED , HIGH);
    }
    else if (receive_buffer == "off")
    {
        digitalWrite(StatusLED, LOW);
    }
}