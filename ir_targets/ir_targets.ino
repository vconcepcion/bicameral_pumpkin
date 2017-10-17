#include <IRremote.h>

int ledPin = 7;
int ledState = 0;
int interval = 3000;
int b_score = 0;
int r_score = 0;

unsigned long startMillis;
unsigned long elapsedTime;

IRrecv *irrecvs[0];
decode_results results;
void setup()
{
  Serial.begin(9600);
  irrecvs[0] = new IRrecv(13); // IR pin 13
  irrecvs[0]->enableIRIn();  
  pinMode(ledPin, OUTPUT); // LED pin 7
}
 
void loop() {
  toggleLedBasedOnTime();

  if (irrecvs[0]->decode(&results)) {
    if(ledState == 1) {
      unsigned int rawbuf_last = results.rawbuf[results.rawlen-1];
      recordHit(rawbuf_last);
    }

    irrecvs[0]->resume();
  }
}

void turnOffLed()
{
  Serial.println("turnOffLed");
  digitalWrite(ledPin, LOW);
  ledState = 0;
  startMillis = millis();
}

void turnOnLed()
{
  Serial.println("turnOnLed");  
  digitalWrite(ledPin, HIGH);
  ledState = 1;
  startMillis = millis();
}


void toggleLedBasedOnTime() {
  elapsedTime = millis() - startMillis;

  if (elapsedTime > interval) {
    if(ledState == 1)
      turnOffLed();
    else
      turnOnLed();
  }
}

void incrementScore(int rawbuf_last) {
  if(rawbuf_last < 10)
    b_score++;
  else
    r_score++;

  Serial.println("Scores:");
  Serial.print("b_score: ");
  Serial.print(b_score);
  Serial.print(" r_score: ");
  Serial.println(r_score);
}

void recordHit(int rawbuf_last) {
  Serial.print(" rawbuf_last: ");
  Serial.println(rawbuf_last);
  incrementScore(rawbuf_last);
  turnOffLed();
}



