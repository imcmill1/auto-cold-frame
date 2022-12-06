// DECLARE VARS //

    int sensorPin = A1; // analog input pin
    int ledRed = 7;
    int ledBlue = 8;
    
    void setup()
    {
      Serial.begin(9600);  //Start the serial connection with the computer
                           //to view the result open the serial monitor 
      pinMode(ledRed, OUTPUT);
      pinMode(ledBlue, OUTPUT);
    }
     
    void loop() 
    {
     int reading = analogRead(sensorPin);  // read data from transistor
     
     float voltage = reading * 5.0;        // turn signal to voltage
     voltage /= 1024.0; 
     
     float temperatureC = (voltage - 0.5) * 100 ;                    // converting from 10 mv per degree with 500 mV offset
                                                                     // to degrees ((voltage - 500mV) times 100)
     
     float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;         // convert to Fahrenheit
     Serial.print(temperatureF); Serial.println(" degrees F\n");
     if (temperatureF > 80.0){
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledBlue, LOW);
     }

     if (temperatureF < 80.0){
      digitalWrite(ledRed, LOW);
      digitalWrite(ledBlue, HIGH);
     }
     
     delay(1000); //waiting a second
    }
