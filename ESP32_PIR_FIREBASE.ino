#include <FirebaseESP32.h>
#include  <WiFi.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define WIFI_SSID "KERMIT" // Change the name of your WIFI
#define WIFI_PASSWORD "1sampai9" // Change the password of your WIFI
#define FIREBASE_Authorization_key "IA2u91yqVhqwRLpwVMiFKFdXdTbBmMKsZ4HttAuH"
#define FIREBASE_HOST "esp32-sensor-readings-ap-9d70a-default-rtdb.firebaseio.com"

 //Send to Thingspeak
 unsigned long myChannelNumber = 1959204;
 const char * myWriteAPIKey = "5PZ4WG4I9UC2EOMT";
 int number = 0;

FirebaseData firebaseData;
FirebaseJson json;

 int keyIndex = 0;            // your network key Index number (needed only for WEP)
 WiFiClient  client;

//String Data
String data1;
String data2;
String data3;
String data4;
String value1;
String value2;
String value3;
String value4;

//Used Pins
int sensor = 22;
int LED1 = 19;
int LED2 = 18;
int LED3 = 5;
int Relay = 17;

void setup() {
  pinMode(sensor, INPUT); // declare sensor as input
  pinMode(LED1, OUTPUT);  // declare as OUTPUT
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(Relay, OUTPUT);

  Serial.begin(115200);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  baby();
  control();
}

void baby(){
//Send Data To Firebase/Thingspeak
  long state = digitalRead(sensor);
  int x = ThingSpeak.writeField(myChannelNumber, 1, state, myWriteAPIKey);
     if(state == HIGH){
       Serial.println("Motion detected!");
       Firebase.setFloat(firebaseData,"BabyMonitoring/motion", state);
     }
     else {
       Serial.println("Motion absent!");
       Firebase.setFloat(firebaseData,"BabyMonitoring/motion", state);
       }
}

void control(){
//Receive Data from Thunkable

  data1 = Firebase.getString(firebaseData,"/Switch/S1");
  value1 = firebaseData.stringData();
    if (value1 == "1"){ // LED 1
       digitalWrite(LED1, HIGH);
       Serial.println("LED1 ON");
    }
    else if(value1 == "0"){
       digitalWrite(LED1, LOW);
       Serial.println("LED1 OFF");
    }

    data2 = Firebase.getString(firebaseData,"/Switch/S2");
    value2 = firebaseData.stringData();
      if (value2 == "2"){ // LED 2
         digitalWrite(LED2, HIGH);
         Serial.println("LED2 ON");
      }
      else if(value2 == "0"){
         digitalWrite(LED2, LOW);
         Serial.println("LED2 OFF");
      }

     data3 = Firebase.getString(firebaseData,"/Switch/S3");
     value3 = firebaseData.stringData();
       if (value3 == "3"){ // LED 3
          digitalWrite(LED3, HIGH);
          Serial.println("LED3 ON");
       }
       else if(value3 == "0"){
          digitalWrite(LED3, LOW);
          Serial.println("LED3 OFF");
       }

     data4 = Firebase.getString(firebaseData,"/Switch/S4");

     value4 = firebaseData.stringData();
       if (value4 == "4"){ // LED 4
          digitalWrite(Relay, HIGH);
          Serial.println("RELAY ON");
       }
       else if(value4 == "0"){
          digitalWrite(Relay, LOW);
          Serial.println("RELAY OFF");
       }
    
}
