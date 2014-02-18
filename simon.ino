/*
  Simon Says
*/

 
// LED Pins
const int LED = 13;      // main
const int green = 2;     // good/continue
const int yellow = 3;    // Simon
const int red = 4;       // error/wrong
// led array
byte ledPins[] = { green, yellow, red, LED };
const int ledTotal = sizeof(ledPins);
byte gamePins[] = { green, yellow, red };
const int gamePinsTotal = sizeof(gamePins);



// set pin numbers:
const int redBtn = 8;
const int yellowBtn = 9;
const int greenBtn = 10;
byte buttons[] = {redBtn, yellowBtn, greenBtn}; 
const int buttonsTotal = sizeof(buttons);



// game variables
String state = "idle";   // game state
int gameRound = 1;
int pattern[9];
int patternLength = 0;
int playerPattern[9];
int playerPatternLength = 0;



// debouncing madness
long debounceDelay = 50;
// green
int green_buttonState;
int green_lastButtonState = LOW;
long green_lastDebounceTime = 0;
// yellow
int yellow_buttonState;
int yellow_lastButtonState = LOW;
long yellow_lastDebounceTime = 0;
// red
int red_buttonState;
int red_lastButtonState = LOW;
long red_lastDebounceTime = 0;







// setup
// ===================================
void setup() {  
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("===================================");
  Serial.println("|              SIMON              |");
  Serial.println("===================================");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("yes, hello. this is Simon");
  Serial.println();
  Serial.println();

  // make sure things are random
  randomSeed(analogRead(0));

  // initialize the LED pins (loop through LEDs array)
  for(int led = 0; led < ledTotal; led++) {
    pinMode(ledPins[led], OUTPUT);
  }
  
  // initialize buttons & enable pull-up resistors on switch pins
  for (byte i=0; i< buttonsTotal; i++) {
    pinMode(buttons[i], INPUT);
  }
}






// loop
// ===================================
void loop() {

// /*
  // check game state
  // ----------------------------
  // idle
  if(state == "idle"){
    // if btn is pressed -> start game
    if (digitalRead(greenBtn) == HIGH) {
      Serial.println("-- start --");
      alert();
      state = "simon";}
    else { knightRider();} // idle LED state
  }

  // game
  if(state == "simon"){
    // setting a new pattern
    simon();
  }

  if(state == "waiting"){
    turnOn(LED);

    // debouncing madness
    // - literally the example code copied 3 times..
    // all of these call registerInput(color);

    // green
    // ----------------------------
    int green_reading = digitalRead(greenBtn);
    if (green_reading != green_lastButtonState) {green_lastDebounceTime = millis();}    
    if ((millis() - green_lastDebounceTime) > debounceDelay) {
      if (green_reading != green_buttonState) {
        green_buttonState = green_reading;
        if (green_buttonState == HIGH) {registerInput(green);}
      }
    }
    green_lastButtonState = green_reading;

    // yellow
    // ----------------------------
    int yellow_reading = digitalRead(yellowBtn);
    if (yellow_reading != yellow_lastButtonState) {yellow_lastDebounceTime = millis();}    
    if ((millis() - yellow_lastDebounceTime) > debounceDelay) {
      if (yellow_reading != yellow_buttonState) {
        yellow_buttonState = yellow_reading;
        if (yellow_buttonState == HIGH) {registerInput(yellow);}
      }
    }
    yellow_lastButtonState = yellow_reading;

    // red
    // ----------------------------
    int red_reading = digitalRead(redBtn);
    if (red_reading != red_lastButtonState) {red_lastDebounceTime = millis();}    
    if ((millis() - red_lastDebounceTime) > debounceDelay) {
      if (red_reading != red_buttonState) {
        red_buttonState = red_reading;
        if (red_buttonState == HIGH) {registerInput(red);}
      }
    }
    red_lastButtonState = red_reading;
  }

  // victory
  if(state == "victory"){
    alert();
    knightRider();
    delay(500);
    knightRider();
    if (digitalRead(greenBtn) == HIGH) {
      Serial.println("-- start --");
      alert();
      state = "simon";
    }
  }
// */

}


// game functions
// ===================================
void simon() {
  // /* ---------------------------- */
  // /             SIMON              /
  // /* ---------------------------- */
  // the game round determines the number of times each LED is turned on
  //    round 1 - all LEDs will be turned on once
  //    round 3 - all LEDs will be turned on 3 times
  // the order in which they are turned on is randomized
  // the player must match the pattern

  if(gameRound > 3) {
    Serial.println("you won!!!");
    state = "victory";
  } else {
    // make pattern
    generatePattern();
  
    Serial.print("Round: ");
    Serial.println(gameRound);

    // show pattern
    showPattern();

    // reset the user
    playerPatternLength = 0;

    // bring us back to Simon
    state = "waiting";
  }
}

void generatePattern() {
  // each LED gets turned on x game round
  // ----------------------------
  // shit gets weird here.
  // i'd be lying if i said i completely understood what i did here
  // koffee

  int one = patternLength;  // 0, 3, 6
  int two = one+1;          // 1, 4, 7
  int three = one+2;        // 2, 5, 8

  // get first random LED (2, 3, 4)
  int led = random(2, 5);
  pattern[one] = led;

  // 2
  if(led == 2) {
    pattern[two] = random(3, 5);
    // 2, 3
    if(pattern[two] == 3){
      pattern[three] = 4;   // 2, 3, 4
    } 
    // 2, 4
    else {
      pattern[three] = 3;   // 2, 4, 3 
    }
  // 3
  } else if(led == 3){
    // random choice between what happens here
    int coin = random(1, 3);
    if(coin == 1){
      // 3, 4, 2
      pattern[two] = 4;
      pattern[three] = 2;    
    } else {
      // 3, 2, 4
      pattern[two] = 2;
      pattern[three] = 4;
    }
  // 4
  } else if(led == 4){
    pattern[two] = random(2, 4);
    // 4, 3
    if(pattern[two] == 3){
      pattern[three] = 2;   // 4, 3, 2
    }
    // 4, 2
    else {
      pattern[three] = 3;   // 4, 2, 3
    }
  }

  // increment patternLength
  patternLength += 3; // 3,6,9
  // patternLength = gameRound * gamePins; // 3,6,9
}

void showPattern() {
  waveLeft();
  delay(1000);
  // turn on LED
  turnOn(LED);
  Serial.print("blink ");
  Serial.print(gameRound);
  Serial.println(" times");
  for(int j=0; j<gameRound; j++){
    turnOff(LED);
    delay(150);
    turnOn(LED);
    Serial.print(j+1);
    Serial.print("-");
    delay(850);

  }
  Serial.println();
  delay(1000);


  for(int i=0; i<patternLength; i++){
    Serial.print(pattern[i]);
    Serial.print(" - ");
    turnOn(pattern[i]);
    delay(500);
    turnOff(pattern[i]);
    delay(500);
  }  

  // let it sink in
  turnOff(LED);
  delay(500);
}


void registerInput(int led) {
  turnOn(led);
  delay(200);
  turnOff(led);
  
  playerPattern[playerPatternLength] = led;
  playerPatternLength++;

  comparePatterns();

  if(playerPatternLength == patternLength){
    Serial.println("next level");
    gameRound++;
    state = "simon";
  }
}

void comparePatterns() {
  Serial.println();

  for(int i=0; i<playerPatternLength; i++){
    if(playerPattern[i] != pattern[i]){
      error();
    } else {
      Serial.print(playerPattern[i]);
      Serial.print(" - ");
    }
  }
}

void error() {
  Serial.println();
  Serial.println("===================================");
  Serial.println("sorry, but you done goofed.");
  Serial.println("===================================");
  Serial.println();
  alert();
  alert(); // yay copy & paste

  state = "idle";
  gameRound = 1;
  patternLength = 0;
}






// convenience functions
// ===================================
void turnOn(int led) {digitalWrite(led, HIGH);}
void turnOff(int led) {digitalWrite(led, LOW);}






// light effects
// ===================================

void waveLeft() {
  for(int led = 0; led < ledTotal; led++) {
    turnOn(ledPins[led]);
    delay(80);
    turnOff(ledPins[led]);
  }
}
void waveRight() {
  for(int led = ledTotal-1; led >= 0; led--) {
    turnOn(ledPins[led]);
    delay(80);
    turnOff(ledPins[led]);
  }
}

// turn on all leds
void turnOnAllLEDs() {
  for(int led = 0; led < ledTotal; led++) {
    turnOn(ledPins[led]);
  }
}

// turn off all leds
void turnOffAllLEDs() {
  for(int led = 0; led < ledTotal; led++) {
    turnOff(ledPins[led]);
  }
}


// idle
void knightRider() {
  waveLeft();
  waveRight();
}

// alert
void alert() {
  for(int i=0; i<4; i++){
    turnOnAllLEDs();
    delay(500);
    turnOffAllLEDs();
    delay(250); 
  }
}
