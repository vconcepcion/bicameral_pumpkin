#include <IRremote.h>

#define RECEIVERS 2

int receiverPins[] = {8,13};   
int ledPin[] = {7,2}; 
int ledState[] = {0,0};
int scores[] = {0,0};
int recieverState[] = {0, 1};
int interval = 3000;
unsigned long startMillis[2];
unsigned long elapsedTime[2];

IRrecv *irrecvs[RECEIVERS];
decode_results results;
void setup()
{
  Serial.begin(9600);
  irrecvs[0] = new IRrecv(8); // Receiver #0
  irrecvs[1] = new IRrecv(13); // Receiver #1: pin 3


  for (int i = 0; i < RECEIVERS; i++)
    irrecvs[i]->enableIRIn();
  
  pinMode(ledPin[0], OUTPUT);
  pinMode(ledPin[1], OUTPUT);
}
 
void loop() {
  toggleLedBasedOnTime();

  for (int i = 0; i < RECEIVERS; i++){
    
    if (irrecvs[i]->decode(&results)) {
      if(ledState[i] == 1) {
        Serial.println("index:");
        Serial.println(i);
        turnOffLed(i);
        scores[i] += i+1;
        Serial.println("scores[%i]:%i", i, scores[i]);
      }

      unsigned int value = results.value;
      Serial.println(value);
      irrecvs[0]->resume();
    }
  }
}

void turnOffLed(int index)
{
  Serial.println("in turnOffLed");
  digitalWrite(ledPin[index], LOW);
  ledState[index] = 0;
  startMillis[index] = millis();

}

void turnOnLed(int index)
{
  Serial.println("in turnOnLed");  
  digitalWrite(ledPin[index], HIGH);
  ledState[index] = 1;
  startMillis[index] = millis();

}


void toggleLedBasedOnTime() {
  for(int i=0; i<2; i++) {
    elapsedTime[i] = millis() - startMillis[i];

    if (elapsedTime[i] > interval) {
      if(ledState[i] == 1)
        turnOffLed(i);
      else
        turnOnLed(i);
    }
  }
}



