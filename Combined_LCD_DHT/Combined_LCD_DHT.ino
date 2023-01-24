// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2           // DHT Data Pin
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino

// LCD Var Setup
const int rs = 12, en = 11, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//DHT Var Setup
float humidity;       //Humidity Value
float temperature;    //Temperature Value


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Read temperature and humidity from the DHT22
  // and store as humidity and temperature
    humidity = dht.readHumidity();
    temperature = dht.readTemperature(true);
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp (F): ");
  lcd.print(temperature);
  

  //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Temp: ");
    Serial.print(temperature);
    Serial.println(" °F");
    delay(2000); //Delay 2 sec.
}
