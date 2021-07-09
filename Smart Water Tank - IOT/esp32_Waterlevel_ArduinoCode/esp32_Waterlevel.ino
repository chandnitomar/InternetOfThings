#include "ThingSpeak.h"
#include <WiFi.h>

//----------- Enter you Wi-Fi Details---------//
char ssid[] = "D-Link_SHINIGAMI"; //SSID
char pass[] = "chillichilli"; // Password
//-------------------------------------------//

// --------------- Tank details --------------//
const int total_height = 15.6; // Tank height in CM
const int hold_height = 13.5;// Water hold height in CM
//-------------------------------------------//

//----- minutes -----//
float minute = 0.5; // Data update in min.
//------------------//

//----------- Channel Details -------------//
unsigned long Channel_ID = 1358838; // Channel ID
const int Field_number = 1; // To which field to write data (don't change)
const char * WriteAPIKey = "51OYSWFLR8E224DH"; // Your write API Key
// ----------------------------------------//

const int trigger = 2;
const int echo = 5;

int led1 = 25; 
int led2 = 26; 
int led3 = 27; 


long Time;
int x;
int i;
float distanceCM;
float resultCM;
int tnk_lvl = 0;
float sensr_to_wtr = 0.00;
WiFiClient  client;


void setup()
{
  Serial.begin(115200);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  sensr_to_wtr = total_height - hold_height;

  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
 
  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
 internet();
 delay(1000); 
}
void loop()
{
  
  
    Serial.println("System Standby....");
    Serial.print(i);
    Serial.println(" Minutes elapsed.");
    delay(1000);
 
  measure();
  Serial.println(Time);
  Serial.print("Tank Level:");
  Serial.print(tnk_lvl);
  Serial.println("%");
  upload();
}
void upload()
{
  internet();
  x = ThingSpeak.writeField(Channel_ID, Field_number, tnk_lvl, WriteAPIKey);
  if (x == 200)Serial.println("Data Updated.");
  if (x != 200)
  {
    Serial.println("Data upload failed, retrying....");
    delay(15000);
    upload();
  }
}

void measure()
{
  delay(100);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  Time = pulseIn(echo, HIGH);
  distanceCM = Time * 0.034;
  resultCM = distanceCM / 2;
 
  tnk_lvl = map(resultCM, sensr_to_wtr, total_height, 100, 0);
 
  if (tnk_lvl > 100) tnk_lvl = 100;
  if (tnk_lvl < 0) tnk_lvl = 0;

  if(  (tnk_lvl > 66) && (tnk_lvl <= 100)   ) 
{
  digitalWrite(led1, HIGH); 
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
} else
if(  (tnk_lvl > 33) && (tnk_lvl <= 66)  ) 
{

  digitalWrite(led1, LOW); 
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
 } else

if(  (tnk_lvl > 0) && (tnk_lvl <= 33)  ) 
{

  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
 } 
 else

if(  tnk_lvl == 0 ) 
{

  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
 } 
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}
