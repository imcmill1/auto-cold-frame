//**************************************************\\
//                                                  \\
// This is a functional user interface that allows  \\
// a user to adjust the upper and lower bounds of   \\
// a temperature range. In practical use, it will   \\
// recieve temperature readings from a DHT22 sensor \\
// ************************************************ \\
//                                                  \\
//  Programmer: Aidan Mizwicki                      \\
//                                                  \\
//**************************************************\\

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
const int rs = 14, en = 13, d4 = 2, d5 = 11, d6 = 10, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonDown = 1;
const int buttonUp = 4;
const int buttonMenu = 3;

unsigned long buttonPressStart;
unsigned long buttonPressDuration;

int menuState;
int lastMenuState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int temperature = 70;
int temperatureUpper = 72;
int temperatureLower = 65;


// This function is used to display the temperature //
// to the user on the LCD display. It can be    //
// messed with to get a good looking display    //
void display_temp(){
  
    // clear the LCD screen
  lcd.clear();
  
    // Print the temperature to the screen
  lcd.print("Temp: ");
  lcd.print(temperature);
}

void setup() {
  //configure lcd contrast
  analogWrite(6, 75);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // set up the button pins as an inputs:
  pinMode(buttonMenu, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  
  // print the temperature reading from the DHT22
  lcd.print("Temp: ");
  lcd.print(temperature);
}

void loop() {
  delay(300);
  //take initial reading of menu button state
  int menuReading = digitalRead(buttonMenu);

  //if the reading doesn't match the last state, something changed 
  if (menuReading != lastMenuState) {
    //reset the debounce timer
    lastDebounceTime = millis();
  }

  //check if it has been longer than the debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    //check if the button state has changed
    if (menuReading != menuState) {
      //update the menuState
      menuState = menuReading;

      //if the new menuState is HIGH, increment temperature as a test
      if (menuState == HIGH) {
        temperature++;
      }
    }
  }

  display_temp();
}
