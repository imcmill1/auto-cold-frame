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
#define actuatorEnable 0
#define driverInput1 6
#define driverInput2 7
#define ledRed 3
#define ledBlue 4
#define actuatorInterval 4175 //intervale is ~4.175sec/15 degree travel increments
#define upperTemp 74
#define lowerTemp 67
#define DHTPIN 5           // DHT Data Pin
#define DHTTYPE DHT22      // DHT Type

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino


// LCD Variable Setup
const int rs = 14, en = 13, d4 = 2, d5 = 11, d6 = 10, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// DHT22 Varibale Setup
float humidity;       //Humidity Value
float temp;           //Temperature Value

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
    
  //print the results to LCD Display
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp (F): ");
  lcd.print(temp); 
      
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
