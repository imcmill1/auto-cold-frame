/*Main_Control.ino
 * 
 * This program serves as the control program for the auto cold frame project.
 * Testing can be done in separate sketches, and this file should serve as the final space
 * for working code.
 * 
 */
// include the libraries for DHT and LCD:
#include <LiquidCrystal.h>
#include <DHT.h>

//preprocessor directives for our global constants
#define ACTUATORSPEED 200
#define actuatorEnable A7
#define driverInput1 A6
#define driverInput2 A5
#define actuatorInterval 4175 //intervale is ~4.175sec/15 degree travel increments
#define DHTPIN 5           // DHT Data Pin
#define DHTTYPE DHT22      // DHT Type

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino

//Temp Limit Setup
int upperTemp = 72;
int lowerTemp = 65;

// LCD Variable Setup
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int contrast = 75;

// DHT22 Variable Setup
float humidity;       //Humidity Value
float temp;           //Temperature Value

//Button Variable Setup
const int buttonDown = 2;
const int buttonUp = 4;
const int buttonMenu = 3;
unsigned long buttonPressStart;
unsigned long buttonPressDuration;

//enumerating state variables
enum positionStates {
 deg0,
 deg15,
 deg30,
 deg45,
 deg60
};

enum positionStates lidState;

bool alertFlag;

void setup() {
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  // Print a startup message
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  
  // Start the DHT22
  dht.begin();
  
  alertFlag = false;
  
  pinMode(actuatorEnable, OUTPUT);
  pinMode(driverInput1, OUTPUT);
  pinMode(driverInput2, OUTPUT);
  
  actuatorConfig();
  retractAct();
  delay(40000);
  stopAct();
  lidState = deg0;
}

void loop() {
  // Read temperature and humidity from the DHT22
  // and store as humidity and temperature
    humidity = dht.readHumidity();
    temp = dht.readTemperature(true);
    


  // check if the button is pressed:
    if (digitalRead(buttonMenu) == HIGH) {
      //if pressed call button press function
      button_press();
      //if longer than 3 seconds
        if (buttonPressDuration >= 3000){
          //run temp upper first, takes over execution and freezes program
          temp_upper();
          //then runs temp lower
          temp_lower();

          //EXECUTION RETURNED TO MAIN LOOP
        }
    }

  //print the data to LCD Display
  display_data();
      
  //at top of loop, if alertFlag is raised, print out alert
  if (alertFlag) {
    lcd.setCursor(0,0);
    lcd.print("  Warning! Cant "); 
    lcd.setCursor(0,1);
    lcd.print("  Control Temp! ");
  }
                                                                 
  switch (lidState) {
    case deg0:
      //if temp goes higher than ambient, goto deg15
      if (temp > upperTemp) {
        //set actuator to extend
        extendAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg15
        lidState = deg15;
        break;
      }
      //else if temp goes lower than ambient, remain closed, throw alert flag
      else if (temp < lowerTemp) {
        alertFlag = true;
        break;
      }
      //else remain
      else {
        alertFlag = false;
        break;
      }

    case deg15:
      //if temp goes higher than ambient, goto deg30
      if (temp > upperTemp) {
        //set actuator to extend
        extendAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg30
        lidState = deg30;
        delay(1000);
        break;
      }
      //else if temp goes lower than ambient, goto deg0
      else if (temp < lowerTemp) {
        //set actuator to retract
        retractAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg0
        lidState = deg0;
        delay(1000);
        break;
      }
      //else remain
      else break;
      
    case deg30:
      //if temp goes higher than ambient, goto deg45
      if (temp > upperTemp) {
        //set actuator to extend
        extendAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg45
        lidState = deg45;
        delay(1000);
        break;
      }
      //else if temp goes lower than ambient, goto deg15
      else if (temp < lowerTemp) {
        //set actuator to retract
        retractAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg15
        lidState = deg15;
        delay(1000);
        break;
      }
      //else remain
      else break;
      
    case deg45:
      //if temp goes higher than ambient, goto deg60
      if (temp > upperTemp) {
        //set actuator to extend
        extendAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg60
        lidState = deg60;
        delay(1000);
        break;
      }
      //else if temp goes lower than ambient, goto deg30
      else if (temp < lowerTemp) {
        //set actuator to retract
        retractAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg30
        lidState = deg30;
        delay(1000);
        break;
      }
      //else remain
      else break;
      
    case deg60:
      //if temp goes higher than ambient, remain, throw alert flag
      if (temp > upperTemp) {
        alertFlag = true;
        break;
      }
      //else if temp goes lower than ambient, goto deg45
      else if (temp < lowerTemp) {
        //set actuator to retract
        retractAct();
        //delay for 5 sec (duration will need adjusting)
        delay(actuatorInterval);
        //set actuator to stop
        stopAct();
        //set lidState to deg45
        lidState = deg45;
        delay(1000);
        break;
      }
      //else remain
      else {
        alertFlag = false;
        break;
      }
  }

  delay(1000);
}

//The below Actuator functions work as shorthand for writing the correct values to the L298 pins
void extendAct() {
  digitalWrite(driverInput1, HIGH);
  digitalWrite(driverInput2, LOW);
}

void retractAct() {
  digitalWrite(driverInput1, LOW);
  digitalWrite(driverInput2, HIGH);
}

void stopAct() {
  digitalWrite(driverInput1, LOW);
  digitalWrite(driverInput2, LOW);
}

//this simply sets up the actuator pins and initializes the values
void actuatorConfig() {
  analogWrite(actuatorEnable, ACTUATORSPEED);
}

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

// This function is used to display the temp and humidity //
// to the user on the LCD display. It can be messed  //
// with to get a good looking display.   //
void display_data(){
  
  // clear the LCD screen
  lcd.clear();
  
  // Print the temperature to the screen
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp (F): ");
  lcd.print(temp); 

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
