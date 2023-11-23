#include <Arduino.h>
int pin = 12;
int currState = 0;
int prevState;
char letterToPrint = 'a';

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
        if (currState == HIGH)
        {
            if (letterToPrint == 'a')
            {
                letterToPrint = 'b';
            }
            else
                letterToPrint = 'a';
        }
        // save the current state as the last state, for next time through the loop
    }
    Serial.println(letterToPrint);
    prevState = currState;
};
