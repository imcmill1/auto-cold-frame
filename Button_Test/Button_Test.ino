#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonDown = 2;
const int buttonUp = 4;
const int buttonMenu = 3;
unsigned long buttonPressStart;
unsigned long buttonPressDuration;
int temperature = 70;
int upperTemp = 72;
int lowerTemp = 65;
int contrast = 75;

// This function tests to see how long the menu //
// button is pressed. It returns that value to   //
// the main loop.               //
int button_press(){
  
  // if the button is pressed, record the start time:
  buttonPressStart = millis();
  
  // wait for the button to be released:
  while (digitalRead(buttonMenu) == HIGH);
  
  // calculate the duration of the button press:
  buttonPressDuration = millis() - buttonPressStart;
  
  // return the value to the loop  
  return(buttonPressDuration);
}

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

// This function adjusts the upper temperature //
// limit. It stays in this adjusting state     //
// until the user presses the menu button.     //
int temp_upper() {
  
    // ensures the display is in this state until 
    // the menu button is pressed
  while (digitalRead(buttonMenu)==LOW){
      
        // clear the screen and display the upper limit
    lcd.clear();
    lcd.print("Upper Bound: ");
    lcd.print(upperTemp);
    delay(300);
        
          // if the (+) button is pressed, increment
        if (digitalRead(buttonUp)==HIGH){
        upperTemp = upperTemp + 1; 
      }

          // if the (-) button is pressed, decrement
      if (digitalRead(buttonDown)==HIGH){
        upperTemp = upperTemp - 1; 
      }
  // if the menu button is pressed, exit to loop
  if (digitalRead(buttonMenu)==HIGH){
      
        // this delay is essential
        // if not there, it skips the lower function
      delay(1000);
      break;
    }
  }
}

// This function adjusts the lower temperature //
// limit. It stays in this adjusting state     //
// until the user presses the menu button.     //
int temp_lower() {
    
    // ensures the display is in this state until 
    // the menu button is pressed
  while (digitalRead(buttonMenu)==LOW){
      
        // clear the screen and display the lower limit
    lcd.clear();
    lcd.print("Lower Bound: ");
    lcd.print(lowerTemp);
    delay(300);
        
          // if the (+) button is pressed, increment
        if (digitalRead(buttonUp)==HIGH){
        lowerTemp = lowerTemp + 1; 
      }
      
          // if the (-) button is pressed, decrement
      if (digitalRead(buttonDown)==HIGH){
        lowerTemp = lowerTemp - 1; 
      }
    // if the menu button is pressed, exit to loop
  if (digitalRead(buttonMenu)==HIGH){
    break;
    }
  }
}

void setup() {
  // set up the LCD's number of columns and rows:
  analogWrite(6,contrast);
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


  // check if the button is pressed:
    // if it is longer than 3 seconds, go to the 
    // temp_upper function
  if (digitalRead(buttonMenu) == HIGH) {
    button_press();
      if (buttonPressDuration >= 3000){
        temp_upper();
        temp_lower();
        display_temp();
      }
  }
}
