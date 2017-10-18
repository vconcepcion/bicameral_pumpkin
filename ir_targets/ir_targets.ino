#include <IRremote.h>

// Constants
#define BASE_COUNT 2
#define BLUE_LASER 0
#define RED_LASER 1


// Define all the devices as global variables
IRrecv *irrecvs[BASE_COUNT];
int ledPins[] = {7,2};
int receiverPins[] = {8,12};

// Define all time related variables
unsigned long startMillis;
unsigned long elapsedTime;


// State variables
int activeReceiver;

// Store scores
int scores[BASE_COUNT] = {0,0};
decode_results results;

// methods
void setup() {
  Serial.begin(9600);
  initSensors();
  initLeds();
}

void initSensors() {
  
  for (int i = 0; i < BASE_COUNT; i++) {
    irrecvs[i] = new IRrecv(receiverPins[i]);
    irrecvs[i]->enableIRIn();
  }
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
      irrecvs[i]->resume();
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
  // generate random intervel between 1000ms to 3000ms
    return rand() % 2000 + 1000;
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
  digitalWrite(ledPins[index], LOW); 
}

void turnOnReceiver(int index) {
  digitalWrite(ledPins[index], HIGH);
}

void recordHit(int index) {
  unsigned int rawbuf_last = results.rawbuf[results.rawlen-1];
  Serial.print(" rawbuf_last: ");
  Serial.println(rawbuf_last);
  
  if(rawbuf_last < 10)
    scores[BLUE_LASER]++;
  else
    scores[RED_LASER]++;
    
  String logger = "hit index: " + String(index);
  Serial.println(logger);
  turnOffReceiver(index);
  printScores();
}

void printScores() {
  Serial.println("Scores:");
  Serial.print("BLUE: ");
  Serial.print(scores[BLUE_LASER]);
  Serial.print(" RED: ");
  Serial.println(scores[RED_LASER]);
}


