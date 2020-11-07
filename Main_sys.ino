#include <call.h>
#include <gps.h>
#include <GSM.h>
#include <HWSerial.h>
#include <inetGSM.h>
#include <LOG.h>
#include <SIM900.h>
#include <sms.h>
#include <Streaming.h>
#include <WideTextFinder.h>
#include <Servo.h>
#include <dht.h>
#include <SoftwareSerial.h>
#include <GSM.h>

SoftwareSerial mySerial(7,8);
const int sensor_pin = A1;  
int pin_out = 9;
dht DHT;
int c=0;

#define dht_apin A0 
#define servoPin 10
Servo myservo;
Servo s;
int angle = 0;
int temp;
int M_Sensor = A2;
int W_led = 7;
int P_led = 13;
int Flag=0;

void setup()
{
  pinMode(2, INPUT);
  pinMode(3, OUTPUT); 
  pinMode(9, OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(7,INPUT);
  pinMode(10,OUTPUT);
  myservo.attach(servoPin);
  Serial.begin(9600);
  s.attach(22);
}
void loop()
{
  //temp and humidity
  if (digitalRead(2) == HIGH)
  {
  digitalWrite(3, HIGH); 
  delay(10000);  
  digitalWrite(3, LOW);  
  delay(100);
  }
  Serial.begin(9600);
  delay(1000);
  DHT.read11(dht_apin);
  float h=DHT.humidity;
  float t=DHT.temperature; 
  delay(5000);
  Serial.begin(9600);
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  float m=moisture_percentage;
  delay(1000);
  
  if(c>=0)
  {
  mySerial.begin(9600);
  delay(15000);
  Serial.begin(9600);
  delay(1000);
  Serial.print("\r");
  delay(1000);                  
  Serial.print("AT+CMGF=1\r");    
  delay(1000);
  Serial.print("AT+CMGS=\"9400832924\"\r"); //replace X with 10 digit mobile number    
  delay(1000);
  Serial.print((String)"update->"+(String)"Temprature="+t+(String)"Humidity="+h+(String)"Moisture="+m);
  delay(1000);
  Serial.write(0x1A);
  delay(1000); 
  mySerial.println("AT+CMGF=1");//Sets the GSM Module in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"9400832924\"\r"); //replace X with 10 digit mobile number    
  delay(1000);
  mySerial.println((String)"update->"+(String)"Temprature="+t+(String)"Humidity="+h+(String)"Moisture="+m);//message format
  mySerial.println();
  delay(100);
  Serial.write(0x1A);
  delay(1000);
  c++;


  // light intensity adjuster

  int l_value= analogRead(A3);
  if(l_value<395){
    s.write(90);
  }
  else{
    s.write(0);
  }  

  //irrigation system

  myservo.write(180);
  int Moisture = analogRead(M_Sensor); 
  Serial.println(Moisture);

 if (Moisture> 200)   // dry soil
 {     
       if (digitalRead(W_led)==1) //water level
       {
         digitalWrite(13, HIGH);
         Serial.println();
         Serial.println("AT+CMGF=1");  
         delay(3000);
         Serial.println();
         Serial.print("AT+CMGS=");    
         Serial.print("\"+9400832924\"");
         Serial.println();
         delay(3000);
         Serial.print("soil is dry ! activating pump."); 
         delay(4000);
         Serial.println();
         Serial.write(26); 
         while (Moisture>100)
         {
            if (Moisture<120)
            {
              digitalWrite(13,LOW);
              break;
            }
         }
       }
       else
       {
         digitalWrite(13, LOW);
         Flag=1;
       }
    }
     if (Moisture>= 70 && Moisture<=200) //Moist Soil
    { 
      Flag=1;
  }
 
  if (Moisture < 70)  // Soggy soil
  { 
    Serial.println();
    Serial.println("AT+CMGF=1");  
    delay(3000);
    Serial.println();
    Serial.print("AT+CMGS=");    
    Serial.print("\"+9400832924\"");
    Serial.println();
    delay(3000);
    Serial.print("Warning ! soil moisture higher than permissible level");
    delay(4000);
    Serial.println();
    Serial.write(26); 
  }
  if (Flag == 1)
  {
    Serial.println();
    Serial.println("AT+CMGF=1");  
    delay(3000);
    Serial.println();
    Serial.print("AT+CMGS=");    
    Serial.print("\"+9400832924\"");
    Serial.println();
    delay(3000);
    Serial.print("Soil moisture is just fine!");
    delay(4000);
    Serial.println();
    Serial.write(26);
  }
  delay(1000);    
  myservo.write(0);
    }
}
