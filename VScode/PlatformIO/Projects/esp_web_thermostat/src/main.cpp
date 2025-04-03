
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
#include "time.h"


#define LED_R 26
#define LED_G 14
#define t_sensor 22

File myFile;        //SD card 
const int CS = 5;   //SD card

void green_blink();
void red_blink();
void check_flag(float tem);
void http_post(String load);
void WriteFile(const char * path, const char * message); 
void ReadFile(const char * path);
void printLocalTime();


// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Rokon 2.4G";
const char* password = "01303294532";
// NTP stuff
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

// Default Threshold Temperature Value
String inputMessage = "25.0";
String lastTemperature;
String enableArmChecked = "checked";
String inputMessage2 = "true";
DHT dht;

// HTML web page to handle 2 input fields (threshold_input, enable_arm_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Temperature Threshold Output Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2>DS18B20 Temperature</h2> 
  <h3>%TEMPERATURE% &deg;C</h3>
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
  return String();
}

// Flag variable to keep track if triggers was activated or not
bool triggerActive = false;

const char* PARAM_INPUT_1 = "threshold_input";
const char* PARAM_INPUT_2 = "enable_arm_input";

// Interval between sensor readings. Learn more about ESP32 timers: https://RandomNerdTutorials.com/esp32-pir-motion-sensor-interrupts-timers/
unsigned long previousMillis = 0;     
const long interval = 5000;    


void setup() {
  Serial.begin(115200);
  //temperature sensor
  dht.setup(t_sensor);
  //LED pins
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  
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
    return;
  }
  Serial.println("initialization done.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //sensors.requestTemperatures();
    // Temperature in Celsius degrees 
    float temperature = dht.getTemperature();
    Serial.print(temperature);
    Serial.println(" *C");  
    lastTemperature = String(temperature);
    
    // Check if temperature is above threshold and if it needs to trigger output
    if(temperature > inputMessage.toFloat() && inputMessage2 == "true" && !triggerActive){
      String message = String("Temperature above threshold. Current temperature: ") + 
                            String(temperature) + String("C");
      Serial.println(message);
      http_post(String(temperature));
      triggerActive = true;
      WriteFile("/alarm.txt", "Alarm triggered!!");
      ReadFile("/alarm.txt");
      
      red_blink();
    }
    // Check if temperature is below threshold and if it needs to trigger output
    else if((temperature < inputMessage.toFloat()) && inputMessage2 == "true" && triggerActive) {
      String message = String("Temperature below threshold. Current temperature: ") + 
                            String(temperature) + String(" C");
      Serial.println(message);
      
      triggerActive = false;
      WriteFile("/alarm.txt", "Alarm deactivated!!");
      ReadFile("/alarm.txt");
      
      green_blink();
      
    }
    
  }
}

void green_blink(){
  for(int i = 0; i < 5; i++){
    digitalWrite(LED_G, HIGH);
    delay(50);
    digitalWrite(LED_G, LOW);
    delay(50);
  }
 
}
void red_blink(){
  for(int i = 0; i < 5; i++){
    digitalWrite(LED_R, HIGH);
    delay(50);
    digitalWrite(LED_R, LOW);
    delay(50);
  }
 
}
/*
void check_flag(float tem){
float temp = tem;
if(temp > inputMessage.toFloat()){
  triggerActive = true;
  }else{
    triggerActive = false;
  }
}
  */

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

void WriteFile(const char * path, const char * message){

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(path, FILE_APPEND);
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
