/* LED Score Keeper

   This sketch tracks the score between two players: 'R' (red) and 'B' (blue). The
   current score is depicted as two series of red and blue dots which accumulate at
   opposite ends of the LED strip and move toward the center. The first color to
   reach the center wins and triggers a fancy pattern display in the winning color.

 */

#include "FastLED.h"

#define DATA_PIN    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60    // This ain't gonna work unless this number is even
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// LED array
CRGB leds[NUM_LEDS];

// Game state
int p1Score;
int p2Score;
int p1Hue = 0;    // red (starts at "data in" end of LED strip)
int p2Hue = 160;  // blue (starts at "data out" end of LED strip)
enum gameStates { WAITING_FOR_PLAYERS, PREP_GAME, IN_PROGRESS, GAME_OVER } gameState;

int gHue = 0;

void setup() {
  resetGame();

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if (gameState == WAITING_FOR_PLAYERS) {
    displayWaitingForPlayers();
  } else if (gameState == PREP_GAME) {
    displayScore();
  } else if (gameState == IN_PROGRESS) {
    if (p1Score >= NUM_LEDS / 2 || p2Score >= NUM_LEDS / 2) {
      gameState = GAME_OVER;
      Serial.write('O');
    } else {
      displayScore();
    }
  } else if (gameState == GAME_OVER) {
    displayWinner();
  }
}


// Handle messages on serial port
void serialEvent() {
  while (Serial.available()) {
    char ch = Serial.read();
    switch (ch) {
      case 'Q':
        resetGame();
        break;
      case 'P':
        prepGame();
        break;
      case 'S':
        startGame();
        break;
    }
  }
}

void serialEvent1() {
  while (Serial1.available()) {
    char ch = Serial1.read();
    switch (ch) {
      case 'R':
      case 'B':
        updateScore(ch);
        break;
    }
  }
}

void resetGame() {
  p1Score = 0;
  p2Score = 0;
  gameState = WAITING_FOR_PLAYERS;
}

void prepGame() {
  if (gameState == WAITING_FOR_PLAYERS) {
    gameState = PREP_GAME;
  }
}

void startGame() {
  if (gameState == PREP_GAME) {
    gameState = IN_PROGRESS;
  }
}

void updateScore(char ch) {
  if (gameState == IN_PROGRESS) {
    if (ch == 'R') {
      p1Score++;
    } else if (ch == 'B') {
      p2Score++;
    }
  }
}


void displayWaitingForPlayers() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }  // cycle through colors
}

void displayScore() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i <= p1Score) {
      leds[i] = CHSV(p1Hue, 255, 192);
    } else if (i >= NUM_LEDS - p2Score - 1) {
      leds[i] = CHSV(p2Hue, 255, 192);
    } else {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
  delay(500);

  // Blink the players' next available LED
  if (p1Score < NUM_LEDS) {
    leds[p1Score] = CRGB::Black;
  }
  if (p2Score < NUM_LEDS) {
    leds[NUM_LEDS - p2Score - 1] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
}

void displayWinner() {
  // "sinelon" pattern - a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-1 );
  if (p1Score >= p2Score) {
    leds[pos] += CHSV(p1Hue, 255, 192);
  } else {
    leds[pos] += CHSV(p2Hue, 255, 192);
  }

  // Send the 'leds' array out to the actual LED strip
  FastLED.show();
  // Insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
}


