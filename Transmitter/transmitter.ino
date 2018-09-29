#include "LowPower.h"
#include <Encoder.h>
#include <VirtualWire.h>

#define buttonPin 2; 
Encoder myEnc(3, 5);
int time;
const int transmit_pin = 4;
char volUp[1] = {'U'};
char volDown[1] = {'D'};
char trackPause[1] = {'P'};
char trackNext[1] = {'N'};
char trackPrev[1] = {'B'};
long oldPosition, newPosition;

void setup() {
   pinMode(buttonPin, INPUT);
   digitalWrite(buttonPin, HIGH );
   Serial.begin(9600);
   //VirtualWire
   vw_set_tx_pin(transmit_pin);
   vw_setup(2000);

   oldPosition = myEnc.read();
   //Interrupt for low power
   attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, CHANGE);
}

void loop() {
   newPosition = myEnc.read();
   
   int b = checkButton();
   if (b == 1) clickEvent();
   if (b == 2) doubleClickEvent();
   if (b == 3) holdEvent();
   if (b == 4) longHoldEvent();

   if (newPosition < oldPosition){
          time = millis();
          oldPosition = newPosition;
          vw_send((uint8_t *)volDown, 1); //send vol down command
          vw_wait_tx(); 
    }
    if(newPosition > oldPosition){
          time = millis();
          oldPosition = newPosition;
          vw_send((uint8_t *)volUp, 1); //send vol up command
          vw_wait_tx();
    }

    if((millis() >= time + 250) && (buttonPin == HIGH)){
    //if 250 miliseconds passes without any action, run sleep function
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
      Serial.println("SLEEP");
     }
   
}

// Events to trigger
void clickEvent() {
    Serial.println("CLICK");
    vw_send((uint8_t *)trackPause, 1);  //send pause command
    vw_wait_tx(); 
}
void doubleClickEvent() {
    Serial.println("DOUBLE CLICK");
    vw_send((uint8_t *)trackNext, 1); //send next track command
    vw_wait_tx(); 
}
void holdEvent() {
    Serial.println("HOLD");
    vw_send((uint8_t *)trackPrev, 1); //send prev track command
    vw_wait_tx(); 
}
void longHoldEvent() {
   Serial.println("LONG HOLD");
}

void pin_ISR() {
  time = millis();
}
