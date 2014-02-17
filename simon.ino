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

// set pin numbers:
const int button = 8;

// helper vars
int buttonState = 0;     // pushbutton status
String state = "idle";   // game state
int gameRound = 1;




// setup
// ===================================
void setup() {                
  // initialize the LED pins (loop through LEDs array)
  for(int led = 0; led < ledTotal; led++) {
    pinMode(ledPins[led], OUTPUT);
  }
  
  // initialize button
  pinMode(button, INPUT);
  
  // serial read
  Serial.begin(9600);
  Serial.println("yes, hello. this is Simon");
  Serial.println();
}




// loop
// ===================================
void loop() {
  Serial.print("state: ");
  Serial.print(state);


  // check game state
  // ----------------------------
  // game
  if(state == "simon"){
    // setting a new pattern
    Serial.println(" | start new game");
    alert();
    simon();
  }

  // idle
  if(state == "idle"){
    // read the state of the pushbutton value:
    buttonState = digitalRead(button);
    Serial.print(" | button: ");
    Serial.println(buttonState);
   
    // if btn is pressed -> start game
    if (buttonState == HIGH) {
      state = "simon";}
    else {
      knightRider();}
  }

}


// game functions
// ===================================
void simon() {
  Serial.print("Round: ");
  Serial.println(gameRound);
}




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
