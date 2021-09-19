
/*
 RETRO
 Space Invader Game
 Author: Stuff 
 */

// include the library code:
#include <LiquidCrystal.h>
#include "OneButton.h"
using namespace std;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};
String morse[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","...---..."};

String inputted_str = "";

OneButton button(A1,true);

//Timer variables
unsigned long timerStart, timerStart_overall;
unsigned long timerInterval;
unsigned long currentTime;

//Button debouncing variables
int currentReading;
int newReading;

//Edge detection variable
int previousReading;

//message var
String message = "";

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);//Initialization of Serial Port
  delay(1000);

  //Initialise timer
  timerStart = millis();
  timerStart_overall = millis();
  timerInterval = 250;
  
  //Initialize debouncing
  currentReading = digitalRead(A1);
  
  lcd.print("Morse code conv.");
  lcd.setCursor(0,1);
  lcd.print("Please wait 10s ");
  delay(10000);

  lcd.clear();
  button.attachClick(singleClick);
  button.attachLongPressStop(longClick);
  button.attachDoubleClick(stop_code);
}

int debounce(int pin, int pinState, int interval) {
  //Debounce button input
  int newValue = digitalRead(pin);
  if (newValue != pinState) {
    delay(interval);
    newValue = digitalRead(pin);
    if (newValue != pinState) {
      pinState = newValue;
    }
  }
  return pinState;
}

void singleClick() {
  Serial.print("ha");
  inputted_str = inputted_str + ".";
}

void longClick() {
  Serial.print("ho");
  inputted_str = inputted_str + "-";
}

void stop_code() {
  Serial.print("he");
  Serial.print('\n');
  Serial.print(inputted_str);
  lcd.setCursor(0,0);
  for (int i=0; i < inputted_str.length();i++) {
    lcd.print(inputted_str[i]);
  }
  int ind = 0;
  //Allow person to see morse code for 4s
  delay(4000);
  for (int j=0; j < sizeof(morse)/sizeof(morse[0]);j++) {
    if (inputted_str != morse[j]) {
      ind += 1;
    }
    else {
      break;
    }
  }
  //ind = ind % 26;
  Serial.print(ind);
  Serial.print('\n');
  message += alphabet[ind];
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(message);
  inputted_str = "";
}

void loop() {

    //Set current time
    currentTime = millis();
    button.tick();
    delay(10);
    
    if ((currentTime-timerStart) > timerInterval) {

        //overall time
        int overall_time;
        overall_time = currentTime - timerStart_overall;

        //Reset timer
        timerStart = currentTime;

        //Save the current button reading
        previousReading = currentReading;

        //Debounce button
        currentReading = debounce(A1,currentReading,4);
    }
}
  
