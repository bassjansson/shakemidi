#include <Arduino.h>
#include <BLEMidi.h>

#define PUSH_BUTTON_1_PIN 34
#define PUSH_BUTTON_2_PIN 36
#define PUSH_BUTTON_3_PIN 39

void setup()
{
    pinMode(PUSH_BUTTON_1_PIN, INPUT);
    pinMode(PUSH_BUTTON_2_PIN, INPUT);
    pinMode(PUSH_BUTTON_3_PIN, INPUT);

    Serial.begin(115200);

    Serial.println("Initializing bluetooth");
    BLEMidiServer.begin("I love you!");

    Serial.println("Waiting for connections...");
    // BLEMidiServer.enableDebugging(); // Uncomment if you want to see some debugging output from the library
}

void sendNote(int& value, int pin, int note)
{
    int newVal = digitalRead(pin);

    if (value != newVal)
    {
        value = newVal;
        value ?
            BLEMidiServer.noteOff(0, note, 0) :
            BLEMidiServer.noteOn(0, note, 127);

    }
}

void loop()
{
    static int pushButton1 = 0;
    static int pushButton2 = 0;
    static int pushButton3 = 0;

    if (BLEMidiServer.isConnected())
    {
        sendNote(pushButton1, PUSH_BUTTON_1_PIN, 36);
        sendNote(pushButton2, PUSH_BUTTON_2_PIN, 38);
        sendNote(pushButton3, PUSH_BUTTON_3_PIN, 42);

        delay(10);
    }
    else
    {
        delay(100);
    }
}
