/*
  State change detection (edge detection)
   
Often, you don't need to know the state of a digital input all the time,
but you just need to know when the input changes from one state to another.
For example, you want to know when a button goes from OFF to ON.  This is called
state change detection, or edge detection.

This example shows how to detect when a button or button changes from off to on
and on to off.
   
The circuit:
* pushbutton attached to pin 2 from +5V
* 10K resistor attached to pin 2 from ground
* LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)

created  27 Sep 2005
modified 30 Aug 2011
by Tom Igoe

This example code is in the public domain.
   
http://arduino.cc/en/Tutorial/ButtonStateChange

*/

// this constant won't change:
const int washPin = A1;    // the pin that the pushbutton is attached to
const int ledPin = 9;       // the pin that the LED is attached to

// Variables will change:
int flashCounter = 0;   // counter for the number of button presses
int flashState = 0;         // current state of the button
int lastFlashState = 0;     // previous state of the button
int flashing = 0;
int notifySend = 0;

void setup() {
  // initialize the button pin as a input:
  pinMode(washPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {

readStatus();

if(flashing == 1 && notifySend == 0) {
  Serial.println("4 flashes, send notification");
  notifySend = 1;
}


if (led full) {     //<----------------- Help?
  notifySend = 0;
}
 
}

void readStatus() {
  // read the pushbutton input pin:
  flashState = digitalRead(washPin);
 
  if (flashCounter == 4) {
    flashing = 1;
  }
  // compare the buttonState to its previous state
  if (flashState != lastFlashState) {
    // if the state has changed, increment the counter
    if (flashState == HIGH) {
      // if the current state is HIGH then the button
      // went from off to on:
      flashCounter++;
      Serial.println("on");
      Serial.print("Flashes: ");
      Serial.println(flashCounter);
    }
    else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
  }
  // save the current state as the last state,
  //for next time through the loop
  lastFlashState = flashState;
}
