/*
  Simon Says
*/

 
// LED Pins
// -----------
const int LED = 13;      // main
const int green = 1;     // good/continue
const int yellow = 2;    // Simon
const int red = 3;       // error/wrong
// led array
int ledPins[] = {green, yellow, red, LED};
int ledTotal = 4;        // is there no array.length attr in arduino?..

// set pin numbers:
const int button = 8;     // the number of the pushbutton pin

// helper vars
int buttonState = 0;         // variable for reading the pushbutton status



// setup
// ====================================
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
// ====================================
void loop() {
  knightRider();



  // read the state of the pushbutton value:
//  buttonState = digitalRead(buttonPin);
//
//  // check if the pushbutton is pressed.
//  // if it is, the buttonState is HIGH:
//  if (buttonState == HIGH) {     
//    // turn LED on:    
//    digitalWrite(ledPin, HIGH);  
//  } 
//  else {
//    // turn LED off:
//    digitalWrite(ledPin, LOW); 
//  }
}



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




void turnOnAllLEDs() {
  digitalWrite(LED, HIGH);
  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);
}

void turnOffAllLEDs() {
  digitalWrite(LED, LOW);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
}

