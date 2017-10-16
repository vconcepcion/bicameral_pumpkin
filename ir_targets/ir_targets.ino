#include <IRremote.h>

int receiverPins[] = {8,13};   
int ledPin[] = {7,2}; 
int ledState[] = {0,0};
int score = 0;

int interval = 3000;
unsigned long startMillis[2];
unsigned long elapsedTime[2];

IRrecv receiver(receiverPins[0]);

decode_results output;
 
void setup()
{
  Serial.begin(9600);
  receiver.enableIRIn();  
  pinMode(ledPin[0], OUTPUT);
  pinMode(ledPin[1], OUTPUT);
  pinMode(ledPin[2], OUTPUT);
  }
 
void loop() {
//  Serial.println("IN Loop:"); 
//  Serial.println("ledState[0]:");
//  Serial.println(ledState[0]);
//  Serial.println("ledOnAt[0]:");
//  Serial.println(ledOnAt[0]);
//  Serial.println("millies:");
//  delay(100);
  toggleLedBasedOnTime();
  
  if (receiver.decode(&output)) {
    if(ledState[0] == 1) {
      turnOffLed(0);
      score++;
      Serial.println("score:");
      Serial.println(score);
    }

    unsigned int value = output.value;
    Serial.println(value);
    receiver.resume();
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



