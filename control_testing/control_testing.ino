/* Control Loop Testing Zone
*/

// include the libraries for DHT, LCD and debouncing:
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Bounce2.h>

//preprocessor directives for our global constants
#define ACTUATORSPEED 200
#define actuatorEnable 0
#define driverInput1 6
#define driverInput2 7
#define ledRed 3
#define ledBlue 4
#define actuatorInterval 4175 //intervale is ~4.175sec/15 degree travel increments
#define DHTPIN 5           // DHT Data Pin
#define DHTTYPE DHT22      // DHT Type
//Buttons are on analog pins acting as GPIO
#define upButtonPin A1
#define downButtonPin A2
#define menuButtonPin A3

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino


// LCD Variable Setup
const int rs = 14, en = 13, d4 = 2, d5 = 11, d6 = 10, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DHT22 Variable Setup
float humidity;       //Humidity Value
float temp;           //Temperature Value

int upperTemp = 74;
int lowerTemp = 67;

//instantiate button objects in bounce2 library
Bounce2::Button upButton = Bounce2::Button();
Bounce2::Button downButton = Bounce2::Button();
Bounce2::Button menuButton = Bounce2::Button();

//button timer variable setup
unsigned long pressStart = 0;
unsigned long pressDuration = 0;

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

//Default setup function
void setup() {
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  // Print a startup message
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  
  // Start the DHT22
  dht.begin();

  //set up pins
  pinMode(actuatorEnable, OUTPUT);
  pinMode(driverInput1, OUTPUT);
  pinMode(driverInput2, OUTPUT);
  pinMode(upButtonPin, INPUT);
  pinMode(downButtonPin, INPUT);
  pinMode(menuButtonPin, INPUT);
  
  //set up the buttons
  upButton.attach(upButtonPin, INPUT);
  downButton.attach(downButtonPin, INPUT);
  menuButton.attach(menuButtonPin, INPUT);

  upButton.interval(5);
  downButton.interval(5);
  menuButton.interval(5);

  upButton.setPressedState(LOW);
  downButton.setPressedState(LOW);
  menuButton.setPressedState(LOW);

  alertFlag = false;
  
  actuatorConfig();
  retractAct();
  //delay(40000);
  stopAct();
  lidState = deg0;
}

//default loop function
void loop() {
  //DEBUG STATEMENTS
  /*Serial.println(digitalRead(upButton))
  Serial.println(digitalRead(downButton))
  Serial.println(digitalRead(menuButton))
  
  Serial.println(pressDuration);*/

  //update buttons
  upButton.update();
  downButton.update();
  menuButton.update();
  
  // Read temperature and humidity from the DHT22
  // and store as humidity and temperature
  humidity = dht.readHumidity();
  temp = dht.readTemperature(true);

  if (menuButton.pressed()) {
    pressStart = millis();
  }

  else if (!menuButton.pressed()) {
    pressDuration = millis() - pressStart;
    if (pressDuration >= 3000) {
      pressDuration = 0;
      adjustTemp();
    }
    else {
      pressDuration = 0;
      pressStart = 0;
    }
  }
  
  //print the current data to LCD Display
  display_data();
   
  //at top of loop, if alertFlag is raised, print out alert
  if (alertFlag) {
    lcd.setCursor(0,0);
    lcd.print("  Warning! Cant "); 
    lcd.setCursor(0,1);
    lcd.print("  Control Temp! ");
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, HIGH);
  }
  else {
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, LOW);
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

// This function is used to display the temperature     //
// and humidity to the user on the LCD display.         //
// It can be messed with to get a good looking display. //
void display_data(){
  
    // clear the LCD screen
  lcd.clear();
  
    // Print the temperature to the screen
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("F");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
}


/*adjustTemp is the temperature adjustment subroutine.
 * It takes over the microcontroller until the user presses the menu
 * button twice, locking in the upper and then the lower temperature limit.
 * After this is done, the limits are moved and control is returned to the main loop.
 */
void adjustTemp() {
  //start by clearing the LCD
  lcd.clear();
  while (!menuButton.pressed()) {
    //Display upper temp limit:
    lcd.setCursor(0,1);
    lcd.print("Upper limit: ");
    lcd.print(upperTemp);
    lcd.print("F");
    //display prompt for change
    lcd.setCursor(0,0);
    lcd.print("Use arrow keys ^");

    if (upButton.pressed()) upperTemp++;
    if (downButton.pressed()) upperTemp--;
  }
  
  //clear LCD between adjustments
  lcd.clear();
  
  while (!menuButton.pressed()) {
    //Now display lower temp limit:
    lcd.setCursor(0,1);
    lcd.print("Lower limit: ");
    lcd.print(lowerTemp);
    lcd.print("F");
    //display prompt for change
    lcd.setCursor(0,0);
    lcd.print("Use arrow keys ^");

    if (upButton.pressed()) lowerTemp++;
    if (downButton.pressed()) lowerTemp--;
  }
  //return control to loop
  return;
}

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

void actuatorConfig() {
  analogWrite(actuatorEnable, ACTUATORSPEED);
}
