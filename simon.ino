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
String state = "idle";   // game state
int buttonState = 0;     // pushbutton status
// debouncing code
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 100;    // the debounce time; increase if the output flickers

// game variables
int gameRound = 1;
int pattern[3];
int patternLength = 0;
int playerPattern[1];
int playerPatternLength = 0;



// setup
// ===================================
void setup() {
  // initialize the LED pins (loop through LEDs array)
  for(int led = 0; led < ledTotal; led++) {
    pinMode(ledPins[led], OUTPUT);
  }
  
  // initialize redBtn
  pinMode(redBtn, INPUT);
  pinMode(yellowBtn, INPUT);
  pinMode(greenBtn, INPUT);
  
  // make sure things are random
  randomSeed(analogRead(0));
  
  // serial read
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
}




// loop
// ===================================
void loop() {

  // check game state
  // ----------------------------
  // idle
  if(state == "idle"){
    // read the state of the pushbutton value:
    buttonState = digitalRead(greenBtn);
   
    // if btn is pressed -> start game
    if (buttonState == HIGH) {
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

    // wait for user input
    if(pushed(greenBtn)){
      checkInput(green);
    } else if(pushed(yellowBtn)){
      checkInput(yellow);
    } else if(pushed(redBtn)){
      checkInput(red);
    }
  }

  // victory
  if(state == "victory"){
    alert();
    knightRider();
    delay(500);
    knightRider();
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
  delay(500);  
  // turn on LED
  turnOn(LED);
  delay(2000);


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


void checkInput(int led) {
  Serial.println();
  Serial.print("pushed LED: ");
  Serial.println(led);
  
  turnOn(led);
  delay(250);

  // playerPattern[playerPatternLength] = led;
  // playerPatternLength++;


  // Serial.println("player:");
  // for(int i=0; i<playerPatternLength; i++){
  //   Serial.print(playerPattern[i]);
  //   Serial.print(" - ");
  // }

  turnOff(led);
}





// debouncing
// ===================================
void check_switches()
{// https://www.adafruit.com/blog/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime) {
     // we wrapped around, lets just try again
     lasttime = millis();
  }
  
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  for (index = 0; index < NUMBUTTONS; index++) {
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
    /*     
    Serial.print(index, DEC);
    Serial.print(": cstate=");
    Serial.print(currentstate[index], DEC);
    Serial.print(", pstate=");
    Serial.print(previousstate[index], DEC);
    Serial.print(", press=");
    */
    
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}






// convenience functions
// ===================================
boolean pushed(int btn) {
  int reading = digitalRead(btn);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    Serial.println();
    Serial.print(reading);
    Serial.print(" - ");
    Serial.print(buttonState);

    // if the button state has changed:
    if (reading != buttonState) {
      Serial.println("idk");

      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println("push");
        return true;
      } else {
        return false;
        Serial.println("--");
      }
    }
  }

  // if(reading == LOW){
  //   return false;
  // } else {
  //   // check to see if you just pressed the button 
  //   // (i.e. the input went from LOW to HIGH),  and you've waited 
  //   // long enough since the last press to ignore any noise:  

    
  //     }
  //   }
    

  //   // save the reading.  Next time through the loop,
  //   // it'll be the lastButtonState:
  //   lastButtonState = reading;

  //   return false;
  // }
}


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





