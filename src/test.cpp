#include <Arduino.h>
int pin = 12;
int currState = 0;
int prevState;

void setup()
{
    Serial.begin(9600);
    pinMode(pin, INPUT);
};

void loop()
{
    // read the pushbutton input pin:
    currState = digitalRead(pin);

    // compare the buttonState to its previous state
    if (currState != prevState)
    {
        // if the state has changed, increment the counter
        switch (currState)
        {
        case HIGH:
            if (currState == 0)
            {
                Serial.println('a');
            }
            else
                Serial.println('b');
            break;

        default:
            break;
        }
        // save the current state as the last state, for next time through the loop
        currState = prevState;
    }
};
