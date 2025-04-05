
/*
 Author: S M Rokonuzzaman/
  SD Card Interface code for ESP32
  SPI Pins of ESP32 SD card as follows:
  CS    = 5;
  MOSI  = 23;
  MISO  = 19;
  SCK   = 18; 
  -------------
  DHT11 Data_pin = 22;

*/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"
#include <HTTPClient.h>
#include <SPI.h>
#include <SD.h>
#include "time.h" // ntp time
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_R 26
#define LED_G 14
#define t_sensor 22
#define I2C_SDA 32
#define I2C_SCL 33

File myFile;        //SD card 
const int CS = 5;   //SD card

void green_blink(int times);
void red_blink(int times);
void http_post(String load);
void WriteFile(const char * path, const char * message, const char * mode); 
void ReadFile(const char * path);
void printLocalTime();
void lcd_print(int x, int y, const char * msg);


// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Rokon 2.4G";
const char* password = "01303294532";
// NTP stuff
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 0;

// Default Threshold Temperature Value
String inputMessage = "35.0";
String lastTemperature,lastHumidity;
String enableArmChecked = "checked";
String inputMessage2 = "true";
DHT dht;

// HTML web page to handle 2 input fields (threshold_input, enable_arm_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Temperature Threshold Output Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2>DHT11 Temperature</h2> 
  <h3>%TEMPERATURE% &deg;C</h3>
  <h4>%HUMIDITY%</H4>
  <h2>ESP Arm Trigger</h2>
  <form action="/get">
    Temperature Threshold <input type="number" step="0.1" name="threshold_input" value="%THRESHOLD%" required><br>
    Arm Trigger <input type="checkbox" name="enable_arm_input" value="true" %ENABLE_ARM_INPUT%><br><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return lastTemperature;
  }
  else if(var == "THRESHOLD"){
    return inputMessage;
  }
  else if(var == "ENABLE_ARM_INPUT"){
    return enableArmChecked;
  }
  else if(var == "HUMIDITY"){
    return lastHumidity;
  }
  return String();
}

// Flag variable to keep track if triggers was activated or not
bool triggerActive = false;

const char* PARAM_INPUT_1 = "threshold_input";
const char* PARAM_INPUT_2 = "enable_arm_input";


unsigned long previousMillis = 0;     
const long interval = 5000;    


LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(115200);
  // LCD section
  Wire.begin(I2C_SDA,I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("I am alive!");


  //temperature sensor
  dht.setup(t_sensor);
  //LED pins
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  lcd_print(0,0,"Wifi connecting!!");
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    lcd_print(0,0,"WiFi Failed!");
    red_blink(5);
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd_print(0,0,"WiFi IP:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  green_blink(5);
  
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Receive an HTTP GET request at <ESP_IP>/get?threshold_input=<inputMessage>&enable_arm_input=<inputMessage2>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
        enableArmChecked = "checked";
      }
      else {
        inputMessage2 = "false";
        enableArmChecked = "";
      }
    }
    Serial.println(inputMessage);
    Serial.println(inputMessage2);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();

  Serial.println("Initializing SD card...");
  if (!SD.begin(CS)) {
    Serial.println("initialization failed!");
    for(int i = 0; i < 10; i++){
      red_blink(10);
      delay(200);
    }
    return;
  }
  	Serial.println("initialization done.");
			green_blink(2);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Temperature in Celsius degrees 
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
    Serial.print(temperature);
    Serial.println(" *C");  
    lastTemperature = String(temperature);
    lastHumidity = String(humidity);
    
    // Check if temperature is above threshold and if it needs to trigger output
    if(temperature > inputMessage.toFloat() && inputMessage2 == "true" && !triggerActive){
      String message = String("Temperature above threshold. Current temperature: ") + 
                            String(temperature) + String("C");
      Serial.println(message);
      http_post(String(temperature));
      triggerActive = true;
      WriteFile("/alarm.txt", "Alarm triggered!!","FILE_APPEND");
      WriteFile("/temp_data.txt", String(temperature).c_str(),"FILE_APPEND");
      ReadFile("/alarm.txt");
      
      red_blink(5);
    }
    // Check if temperature is below threshold and if it needs to trigger output
    else if((temperature < inputMessage.toFloat()) && inputMessage2 == "true" && triggerActive) {
      String message = String("Temperature below threshold. Current temperature: ") + 
                            String(temperature) + String(" C");
      Serial.println(message);
      
      triggerActive = false;
      WriteFile("/alarm.txt", "Alarm deactivated!!","FILE_APPEND");
      ReadFile("/alarm.txt");
      
      green_blink(5);
      
    }
  }
}

void green_blink(int times){
  for(int i = 0; i < times; i++){
    digitalWrite(LED_G, HIGH);
    delay(50);
		digitalWrite(LED_G, LOW);
    delay(50); 
  }   
}
void red_blink(int times){
  for(int i = 0; i < times; i++){
    digitalWrite(LED_R, HIGH);
    delay(50);
		digitalWrite(LED_R, LOW);
    delay(50);   
  }
}

void http_post(String text){
  //http stuff
  String t = text;
  HTTPClient http;
  String url = "https://api.callmebot.com/whatsapp.php?phone=8801303294532&text=The+Temp+is+"+t+"&apikey=6430232";
  http.begin(url);
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  ///// http stuff ends
}

void WriteFile(const char * path, const char * message,const char * mode){

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(path, mode);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.printf("Writing to %s ", path);
    myFile.println(message);
    myFile.close(); // close the file:
    Serial.println("completed.");
    
  } 
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening file ");
    Serial.println(path);
  }
}

void ReadFile(const char * path){
  // open the file for reading:
  myFile = SD.open(path);
  if (myFile) {
     Serial.printf("Reading file from %s\n", path);
     // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close(); // close the file:
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void lcd_print(int x, int y, const char * msg){
  lcd.setCursor(x,y);
  lcd.print(msg);
}