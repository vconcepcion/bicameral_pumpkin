#include <IRremote.h>

#define BASE_COUNT 2
#define THRESHOLD 1000
// Define all the devices as global variables
IRrecv *irrecvs[BASE_COUNT];
int ledPins[] = {7,2}; 
int receiverPins[] = {8,13};   

// Define all time related variables
unsigned long startMillis;
unsigned long elapsedTime;


// State variables
int activeReceiver;

// Store scores
int score = 0;
decode_results results;

// methods
void setup() {
  Serial.begin(9600);
  initSensors();
  initLeds();
}

void initSensors() {
  irrecvs[0] = new IRrecv(8);
  irrecvs[1] = new IRrecv(13);

  for (int i = 0; i < BASE_COUNT; i++)
    irrecvs[i]->enableIRIn();
}

void initLeds() {
  for (int i = 0; i < BASE_COUNT; i++)
    pinMode(ledPins[i], OUTPUT);
}

void loop() {
  toggleSensors();

  for (int i = 0; i < BASE_COUNT; i++){
    if (irrecvs[i]->decode(&results)) {
      if(i == activeReceiver)
        recordHit(i);
      irrecvs[0]->resume();
    }
  }
}

void toggleSensors() {
  elapsedTime = millis() - startMillis;
  
  if (elapsedTime > interval()) {
    activeReceiver = rand() % BASE_COUNT;
    updateReceiverState();
  }
}

int interval() {
  // generate random intervel between 500ms to 2000ms
  return rand() % 2000 + 500;
}

void updateReceiverState() {
  for (int i = 0; i < BASE_COUNT; i++) {
    if(i == activeReceiver)
      turnOnReceiver(i);
    else
      turnOffReceiver(i);
  }

  startMillis = millis();
}

void turnOffReceiver(int index) {
  String logger = "set receiver " + String(index) + " OFF";
  Serial.print(logger);
  digitalWrite(ledPins[index], LOW); 
}

void turnOnReceiver(int index) {
  String logger = "set receiver " + String(index) + " ON";
  Serial.println(logger);
  digitalWrite(ledPins[index], HIGH);
}

void incrementScore() {
  String logger = "incremented score to: " + String(++score);
  Serial.println(logger);
}

void recordHit(int index) {
  unsigned int value = results.value;
  String logger = "register senser " + String(index) + " hit with value: " + String(value);
  Serial.println(logger);

  if(value > THRESHOLD) {
    incrementScore();
    turnOffReceiver(index);
    startMillis = millis();
  }
}


