#include <EEPROM.h>

int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int whitePin = 3;

int rSwitch = 7;
/*int gSwitch = 8;
int bSwitch = 2;*/
int wSwitch = 4;
int scintSwitch = 5;
int clash = 6;
int ignition = 12;

boolean lit = false; 

float r, g, b, w = 0; // LED intensity
int cWheelPos = 0; // color wheel position (0-359)

int rAddress = 0;
int gAddress = sizeof(float);
int bAddress = sizeof(float)*2;
int wAddress = sizeof(float)*3;

void setup() { //IDLE STATE
  Serial.begin(9600);
  
  pinMode(rSwitch, INPUT_PULLUP);
  pinMode(wSwitch, INPUT_PULLUP);
  pinMode(scintSwitch, INPUT_PULLUP);
  pinMode(clash, INPUT_PULLUP);
  pinMode(ignition, INPUT_PULLUP);

  EEPROM.get(rAddress, r);
  EEPROM.get(gAddress, g);
  EEPROM.get(bAddress, b);
  EEPROM.get(wAddress, w);

  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  analogWrite(whitePin, 0);
}

void loop() {

  if (digitalRead(ignition) == LOW) { //ignition switch
    delay(250); 
    lit = !lit;
    
    if(!lit) {
      EEPROM.put(rAddress, r);
      EEPROM.put(gAddress, g);
      EEPROM.put(bAddress, b);
      EEPROM.put(wAddress, w);
      
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      analogWrite(whitePin, 0);
    }
  }

  if (lit){
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
    analogWrite(whitePin, w); 
  }
  
  if (digitalRead(rSwitch) == LOW) {
    push(redPin, greenPin, bluePin, &r, &g, &b, &cWheelPos);
      delay(20);
  }

  if (digitalRead(scintSwitch) == LOW) {
    scintillate(r, g, b, w);
  }

  if (digitalRead(clash) == LOW) {
    fade(1.75);
  }
  
}

void fade(float factor) {
  
  for(float i = w; i < 256; i += 2*factor){
    analogWrite(whitePin, i);
    delay(1);
  }
  for(float i = 256; i >= w; i -= 2*factor){
    analogWrite(whitePin, i);
    delay(1);
  }
}

//color cycling method
void push(int rPin, int gPin, int bPin, float* rVal, float* gVal, float* bVal, int* cWheelPos){
  //increment color wheel position. if at 360, go back to 0
  if(*cWheelPos != 359){
    *cWheelPos += 1;
  }

  //HSV to RGB. S = V = 1 always, H = cWheelPos
  int x = 255 * ((*cWheelPos % 60)/60.);
  int maxVal = 0;
  float factorA = 0;
  float factorB = 0;

  if(*cWheelPos < 60){
    factorA = 255./(255+x);
    factorB = x/(255.+x);
    maxVal = factorA*255.;
    x = factorB*x;
  
    *rVal = maxVal;
    *gVal = x;
    *bVal = 0;
  } else if(*cWheelPos < 120){
    x = 255 - x;
    factorA = 255./(255+x);
    factorB = x/(255.+x);
    maxVal = factorA*255.;
    x = factorB*x;
    
    *rVal = x;
    *gVal = maxVal;
    *bVal = 0;
  } else if(*cWheelPos < 180){
    factorA = 255./(255+x);
    factorB = x/(255.+x);
    maxVal = factorA*255.;
    x = factorB*x;
  
    *rVal = 0;
    *gVal = maxVal;
    *bVal = x;
  } else if(*cWheelPos < 240){
    x = 255 - x;
    factorA = 255./(255+x);
    factorB = x/(255.+x);
    maxVal = factorA*255.;
    x = factorB*x;
    
    *rVal = 0;
    *gVal = x;
    *bVal = maxVal;
  } else if(*cWheelPos < 300){
    factorA = 255./(255+x);
    factorB = x/(255.+x);
    maxVal = factorA*255.;
    x = factorB*x;
  
    *rVal = x;
    *gVal = 0;
    *bVal = maxVal;
  } else {
    x = 255 - x;
    factorA = 255./(255+x);
    factorB = x/(255.+x);
    maxVal = factorA*255.;
    x = factorB*x;
    
    *rVal = maxVal;
    *gVal = 0;
    *bVal = x;
  }

  if(*cWheelPos == 359){
    *cWheelPos = 0;
  }

  /*analogWrite(rPin, *rVal);
  analogWrite(gPin, *gVal);
  analogWrite(bPin, *bVal);*/
}

void scintillate(float r, float g, float b, float w){

  for(int i = 0; i <= 50; i++){
    analogWrite(redPin, r * (100 - i)/100);
    analogWrite(greenPin, g * (100 - i)/100);
    analogWrite(bluePin, b * (100 - i)/100);
    analogWrite(whitePin, w * (100 - i)/100);
    delay(20);
  }
  for(int i = 50; i >= 0; i --){
    analogWrite(redPin, r * (100 - i)/100);
    analogWrite(greenPin, g * (100 - i)/100);
    analogWrite(bluePin, b * (100 - i)/100);
    analogWrite(whitePin, w * (100 - i)/100);
    delay(20);
  }
  
}

void ignite(){
  //TODO:
  //insert ignition sound
  //commence idle hum
  //move startup stuff into this method if plausible
}

void swing(){
  //TODO:
  //read output of detectMotion()
  //determine if it's enough motion to proc a sound
  //based on the amount of motion, proc long or short swing, or clash
}

void detectMotion(){
  //TODO:
  //capture motion data from saber at all times
  //return the net distance traveled over a certain amount of time
  //if there was sufficient movement in close-enough-to-opposite directions, indicate a clash
  //otherwise indicate a swing or nothing
}
  



