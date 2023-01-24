/* Critical Subassembly Code Demo Program 
   Purpose: An Arduino program that integrates temperature
   sensors with the linear actuator to demonstrate our 
   critical subassembly.

   Author: Ian McMillan
   Version: 0.5
   Last Modified: 12/6/2022
*/


//preprocessor directives for our global constants
#define ACTUATORSPEED 200
#define actuatorEnable 11
#define driverInput1 12
#define driverInput2 13
#define sensorPin A1
#define ledRed 7
#define ledBlue 8
#define actuatorInterval 4175 //intervale is ~4.175sec/15 degree travel increments
#define upperTemp 73
#define lowerTemp 67

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
  //read data from transistor
  int reading = analogRead(sensorPin);
  // turn signal to voltage
  float voltage = reading * 5.0;        
  voltage /= 1024.0; 
  // converting from 10 mv per degree with 500 mV offset to degrees ((voltage - 500mV) times 100)
  float temperatureC = (voltage - 0.5) * 100 ; 
  //converting to Fahrenheit 
  float temp = (temperatureC * 9.0 / 5.0) + 32.0;     
  //printing result to console
  Serial.print(temp); Serial.println(" degrees F\n");     
  //at top of loop, if alertFlag is raised, print out alert
  if (alertFlag) {
    Serial.println("Alert! Cannot control temperature!"); 
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

