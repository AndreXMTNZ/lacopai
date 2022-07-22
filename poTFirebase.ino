//Libraries
#include "Firebase_Arduino_WiFiNINA.h"
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
//Conections to Database of Firebase and Network TCP/IP
/*ZyXEL NBG-418N v2*//*ElWifiDel3B22K22*//*"Nexxt_BGN11"*//*"FKTUD-nexxt24"*/
#define DATABASE_URL "smart-tank-iot-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "wW9ayoCtblIkDzhA86Z5iUXSoaDZC2CNwnxv4Bvb"
#define WIFI_SSID "Nexxt_BGN11"
#define WIFI_PASSWORD "FKTUD-nexxt24"

//Defs for Sensor TDS
#define TdsSensorPin A3
#define VREF 3.3 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point

//Vars to temperature sensor.
OneWire ourWire(10);                //Se establece el pin 2 como bus OneWire
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor

//Define Firebase data object
FirebaseData fbdo;

//Vars to wlevel, TDS & temp.
//Use reles on pin 0 to 7.
int nivel = 9;//valor de lectura

int r1 = 1;//Water Level
int r2 = 2;//TDS
int r3 = 3;//temp HEAT
int r4 = 4;//LAMP1
int r5 = 5;//LAMP1

//Vars to TDS.
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;

//Vars to pH Sensor.
float calibration_value = 21.34;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

void setup() {
  Serial.begin(9600);
  pinMode(nivel, INPUT);

  pinMode(r1, OUTPUT);//Water Level
  pinMode(r2,OUTPUT);//TDS
  pinMode(r3, OUTPUT);//Temp heat
  pinMode(r4,OUTPUT);//Lamp 1
  pinMode(r5, OUTPUT);//Lamp 2

  WiFiDrv::pinMode(25, OUTPUT);
  WiFiDrv::pinMode(26, OUTPUT);
  WiFiDrv::pinMode(27, OUTPUT);
  sensors.begin();   //Se inicia el sensor
  pinMode(TdsSensorPin,INPUT);
  delay(1000);

  Serial.print("Connecting to WiFi…");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(300);
  }
  Serial.print(" IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Provide the autntication data
  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase conectado");
}

void loop() {

//For data from pH Sensor.
  /*float ph = 0;
  if (Firebase.getFloat(fbdo, "/test/ph")) {
    if (fbdo.dataType() == "float") {
      ph = fbdo.floatData();
      Serial.print("PH");
      Serial.println(ph);
    }
  } else {
    //Failed, then print out the error detail
    Serial.print("Error: ");
    Serial.println(fbdo.errorReason());
  }*/


int read1 = level();
level();

int read2 = tempa();
tempa();  

int read3 = tdss();
tdss();

float read4 = ph();
ph();

//Code for TDS and Firebase.

   //Upload data from water level.
  if (Firebase.setFloat(fbdo, "/test/ph", read4)){

    if (fbdo.dataType() == "float")
      Serial.println(fbdo.floatData());

  } else {
    //Failed, then print out the error detail
    Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
  }

  //Upload data from water level.
  if (Firebase.setInt(fbdo, "/test/nivel", read1)){

    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());

  } else {
    //Failed, then print out the error detail
    Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
  }

  //Upload data from temp.
  if (Firebase.setInt(fbdo, "/test/temp", read2)){

    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());

  } else {
    //Failed, then print out the error detail
    Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
  }

  //Upload data from TDS.
  if (Firebase.setInt(fbdo, "/test/tds", read3)){

    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());

  } else {
    //Failed, then print out the error detail
    Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
  }

  //For get data for lamp 1.
  bool bot1 = false;
  if (Firebase.getBool(fbdo, "/config/bot1")) {
    if (fbdo.dataType() == "bool") {
      bot1 = fbdo.boolData();
      Serial.print("Lumunaria 1");
      Serial.println(bot1);

    }
  } else {
      //Failed, then print out the error detail
      Serial.print("Error: ");
      Serial.println(fbdo.errorReason());
  }      
  
  if(bot1 == false){
    digitalWrite(r4, HIGH);
  }

  if(bot1 == true){
    digitalWrite(r4, LOW);
  }

  delay(500);
}