/*
  Simon Says
*/

 
// LED Pins
const int LED = 13;      // main
const int green = 2;     // good/continue
const int yellow = 3;    // Simon
const int red = 4;       // error/wrong
// led array
int ledPins[] = { green, yellow, red, LED };
int ledTotal = 4;        // is there no array.length attr in arduino?..
int gamePins[] = { green, yellow, red };
int gamePinsTotal = 3;

// set pin numbers:
const int redBtn = 8;
const int yellowBtn = 9;
const int greenBtn = 10;

// helper vars
int buttonState = 0;     // pushbutton status
String state = "idle";   // game state

// game variables
int gameRound = 1;
int pattern[] = { green };
int playerPattern[] = { green };




// setup
// ===================================
void setup() {
  // initialize the LED pins (loop through LEDs array)
  for(int led = 0; led < ledTotal; led++) {
    pinMode(ledPins[led], OUTPUT);
  }
  
  // initialize redBtn
  pinMode(redBtn, INPUT);
  // pinMode(yellowBtn, INPUT);
  // pinMode(greenBtn, INPUT);
  
  // serial read
  Serial.begin(9600);
  Serial.println("===================================");
  Serial.println("|              SIMON              |");
  Serial.println("===================================");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("yes, hello. this is Simon");
}




// loop
// ===================================
void loop() {

  // check game state
  // ----------------------------
  // game
  if(state == "simon"){
    // setting a new pattern
    simon();
  }

  // idle
  if(state == "idle"){
    // read the state of the pushbutton value:
    buttonState = digitalRead(redBtn);
   
    // if btn is pressed -> start game
    if (buttonState == HIGH) {
      alert();
      state = "simon";}
    else { knightRider();} // idle LED state
  }
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

  Serial.print("Round: ");
  Serial.println(gameRound);

  // set pattern
  showPattern();

  // bring us back to Simon
  state = "simon";
}

void showPattern() {
  // turn on LED
  turnOn(LED);

  // make pattern
  int length = gameRound * gamePinsTotal;
  for(int i=0; i<length; i++){
    Serial.println(i+1);
  }
}

void turnOn(int _led) {digitalWrite(_led, HIGH);}
void turnOff(int _led) {digitalWrite(_led, LOW);}


// light effects
// ===================================

// idle
void knightRider() {
  //  knight rider
  for(int led = 0; led < ledTotal; led++) {
    digitalWrite(ledPins[led], HIGH);
    delay(80);
    digitalWrite(ledPins[led], LOW);
  }
  for(int led = ledTotal-1; led >= 0; led--) {
    digitalWrite(ledPins[led], HIGH);
    delay(80);
    digitalWrite(ledPins[led], LOW);
  }
}

// alert
void alert() {
  for(int i=0; i<3; i++){
    turnOnAllLEDs();
    delay(500);
    turnOffAllLEDs();
    delay(250); 
  }
}

// turn on all leds
void turnOnAllLEDs() {
  for(int led = 0; led < ledTotal; led++) {
    digitalWrite(ledPins[led], HIGH);
  }
}

// turn off all leds
void turnOffAllLEDs() {
  for(int led = 0; led < ledTotal; led++) {
    digitalWrite(ledPins[led], LOW);
  }
}



