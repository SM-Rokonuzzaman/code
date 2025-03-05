#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "WiFi.h"
#include "time.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>


LiquidCrystal_I2C lcd(0x27,16,2);

const char* ssid = "Rokon 2.4G";
const char* pass = "01303294532";
const char* ntp = "pool.ntp.org";

const char* api_link = "https://api.openweathermap.org/data/2.5/weather?lat=23.8041&lon=90.4152&appid=c26bbe8c705ee0c495792e5c836656a8";

const long gmtoffset_sec = 21600; //GMT +6
const long daylightoffset_sec = 0;
String json;
float feels_like;
bool now = true;

void setup() {

Serial.begin(9600);
lcd.init();
lcd.backlight();
lcd.noBacklight();

WiFi.mode(WIFI_STA);
WiFi.begin(ssid,pass);
lcd.print("Connecting to WIFI.");
lcd.setCursor(0,1);

while (WiFi.status()!= WL_CONNECTED)
{
  Serial.printf(".");
  lcd.print(".");
}

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Wifi Connected!");
delay(2000);

configTime(gmtoffset_sec,daylightoffset_sec, ntp);




}

void json_dserealize(){

  JsonDocument doc;    //declaring a jon doc
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  
  feels_like = doc["main"]["feels_like"];
 

  // Print values.
  Serial.println("Feels Like: ");
  Serial.println(feels_like);



}
void http_post(){
  HTTPClient http;
  http.begin(api_link);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.GET();
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
      json = payload;


    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  

  http.end();
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

  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print(&timeinfo, "%I");

  lcd.setCursor(6,0);
  lcd.print(":");
  lcd.setCursor(7,0);
  lcd.print(&timeinfo, "%M");
  lcd.setCursor(9,0);
  lcd.print(":");
  lcd.setCursor(10,0);
  lcd.println(&timeinfo, "%S");
  lcd.setCursor(12,0);
  lcd.print("  ");
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



void loop() {

  printLocalTime();
  
  if(now){
    http_post();
    now = false;
  }
  
  json_dserealize();
  float tem = feels_like - 273.00;
  lcd.setCursor(0,1);
  lcd.print("Feels Like: ");
  lcd.setCursor(11,1);
  lcd.print(tem);

  delay(1000);

}


