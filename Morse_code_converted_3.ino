
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

//have arrays for both the alphabet and morse code symbols
char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};
String morse[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","......."};

//Create a string variable which stores the message in morse code
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

  //Print some into messages including asking the person to wait 10s
  lcd.print("Morse code conv.");
  lcd.setCursor(0,1);
  lcd.print("Please wait 10s ");
  delay(10000);

  //Clear the screen and attach the following functions to the associated functions
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
  //Add a dot (a dit in morse code) to the message in morse code
  inputted_str = inputted_str + ".";
}

void longClick() {
  //Add a dash (a dah in morse code) to the message in morse code
  inputted_str = inputted_str + "-";
}

void stop_code() {
  //Reset the cursor position and print the message in morse code for the user to reread
  lcd.setCursor(0,0);
  for (int i=0; i < inputted_str.length();i++) {
    lcd.print(inputted_str[i]);
  }
  //Define invalid message string variable
  //Define an index variable
  int ind = 0;
  //Allow person to see morse code for 4s
  delay(4000);
  //Define the length of the morse code by making use of the sizeof function as it is a string in the for loop
  for (int j=0; j < sizeof(morse)/sizeof(morse[0]);j++) {
    if (inputted_str != morse[j]) {
      ind += 1;
    }
    else {
      lcd.print("Invalid char");
      return; //Removing break makes this for loop not work
    }
  }
  //The serial print statement is used to double check that the index when applied to the dictionary actually corresponds to the correct alphabet
  Serial.print(ind);
  Serial.print('\n');
  //Add the required alphabet to the message 
  message += alphabet[ind];
  //Print the message to the screen
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
  
