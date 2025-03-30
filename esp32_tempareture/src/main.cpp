/*
  ESP32 DHT11 Code
  http:://www.electronicwings.com
*/

#include "DHT.h"
#define LED_R 26
#define LED_G 14

DHT dht;
float Humidity, Temperature;
float temp_pool[10];
void led_blink();
void red_blink();

void setup(void){
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  Serial.begin(9600);         /*Set the baudrate to 115200*/
  dht.setup(23);                 /*Connect the DHT11's data pin to GPIO23*/
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
  delay(1000);                  /*Wait for 1000mS*/
}

void loop(void){


  for (int i =0; i <5; i++){
    temp_pool[i] = dht.getTemperature();
    Serial.print("T: ");
    Serial.println(temp_pool[i]);
    delay(500);
  }
  float t = 0;
  for(int j = 0; j <5; j++){
    t = t + temp_pool[j];
    
  }
    t = t - t*0.16;
    Serial.printf("Temp: %f\n",t/5);
    if( t/5 < 23.00){
      led_blink();
      
    }else if (t/5 > 23.00){
      red_blink();
      
    }
    

  Temperature = dht.getTemperature(); /*Get the Temperature value*/
  Humidity = dht.getHumidity();       /*Get the Humidity value*/
 
  Serial.print(dht.getStatusString());/*Get the Status of DHT11 Sensor*/
  Serial.print("\t");
  Serial.print(Humidity, 1);
  Serial.print("\t\t");
  Serial.println(Temperature, 1);

 
  delay(100);

  //delay(dht.getMinimumSamplingPeriod()*5);
}

void led_blink(){
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