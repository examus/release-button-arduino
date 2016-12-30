#include <SoftwareSerial.h>
 
SoftwareSerial RfidReader(2, 3);
int num;
int count = 0;
int led = 7;
int data1 = 0;

int buttonPin = 5;

int newtag[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};


int STATE = 0;

const int STATE_INITIAL = 0;
const int STATE_PRESSED = 1;
const int STATE_FIRST_TAG = 2;
const int STATE_SECOND_TAG = 3;
const int STATE_RELEASED = 4;
const int STATE_WRONG = 5;

int ACTION_PRESS_BUTTON = 1;
int ACTION_RELEASE_BUTTON = 2;
int ACTION_INPUT_TAG = 3;

bool BUTTON_PRESSED = false;

int tags[][14] = {
 {2, 48, 57, 48, 48, 50, 49, 53, 53, 50, 49, 53, 67, 3 }, // yellow
 {2, 48, 69, 48, 48, 49, 51, 55, 70, 52, 69, 50, 67, 3 }  // blue

};

int read_tags[2] = {-1, -1};

int process(int state, int key) {
//  Serial.print(state);
//  Serial.print(' ');
//  Serial.println(key);
  int new_state = STATE_WRONG;

  switch (state) {
    case STATE_INITIAL:
      if (BUTTON_PRESSED) {
        new_state = STATE_PRESSED;
      } else {
        new_state = STATE_INITIAL;
      }
      break;

    case STATE_PRESSED:
      if (BUTTON_PRESSED) {// THINK
        if (key != -1) {
          new_state = STATE_FIRST_TAG;
        }
      }
      break;
    case STATE_FIRST_TAG:
      if (BUTTON_PRESSED == true) {
        if (key != -1) {
          new_state = STATE_SECOND_TAG;
        }
      }
      break;
    case STATE_SECOND_TAG:
      if (BUTTON_PRESSED == false) {
        new_state = STATE_RELEASED;
      }
  }
  Serial.println(new_state);
  delay(1000);
  return new_state;
}

bool comparetag(int aa[14], int bb[14]){
 bool ff = false;
 int fg = 0;
 for (int cc = 0 ; cc < 14 ; cc++){
 
  if (aa[cc] == bb[cc]){
   fg++;
  }
 }
 if (fg == 14){
  ff = true;
 }
 return ff;
}


void print_tag(int tag[14]) {
  for (int t = 0; t < 14; t++) {
    Serial.print(tag[t]);
    Serial.print(", ");
  }
  Serial.println(";");
}
 
void setup(){
 pinMode(buttonPin, INPUT);
 Serial.begin(9600); 
 RfidReader.begin(9600);
 Serial.print("Starting...");
}
 
void loop() {
  int buttonState = digitalRead(buttonPin);
//  Serial.println(BUTTON_PRESSED);
  if (buttonState == HIGH) {
    if (BUTTON_PRESSED == false) {
      BUTTON_PRESSED = true;
      STATE = process(STATE, -1);
    }
    
  }
  else {
    if (BUTTON_PRESSED == true) {
      BUTTON_PRESSED = false;
      STATE = process(STATE, -1);
    }
  }
  
 if (RfidReader.available() > 0) {
  Serial.println("read tag");
  for (int x = 0 ; x < 14 ; x++) {
   data1 = RfidReader.read();
   newtag[x] = data1;
  }
  RfidReader.flush();
  print_tag(newtag);
  for(int i = 0; i < sizeof(tags) ; i++){
   if (comparetag(newtag, tags[i]) == true){
    STATE = process(STATE, i);
    break;
   }
  }
 }

  switch (STATE){
    case STATE_INITIAL:
      analogWrite(7, 0);
      analogWrite(8, 0);
      analogWrite(9, 0);
      analogWrite(10, 0);
      break;
    case STATE_PRESSED:
      analogWrite(7, 0);
      analogWrite(8, 0);
      analogWrite(9, 0);
      analogWrite(10, 255);
      break;
    case STATE_FIRST_TAG:
      analogWrite(7, 0);
      analogWrite(8, 0);
      analogWrite(9, 255);
      analogWrite(10, 255);
      break;
    case STATE_SECOND_TAG:
      analogWrite(7, 0);
      analogWrite(8, 255);
      analogWrite(9, 255);
      analogWrite(10, 255);
      break;
    case STATE_RELEASED:
      analogWrite(7, 255);
      analogWrite(8, 255);
      analogWrite(9, 255);
      analogWrite(10, 255);
      break;
    case STATE_WRONG:
      analogWrite(7, 0);
      analogWrite(8, 0);
      analogWrite(9, 0);
      analogWrite(10, 0);
      break;
 };

}
