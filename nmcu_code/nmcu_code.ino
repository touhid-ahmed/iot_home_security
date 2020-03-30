#include <ESP8266WiFi.h>                                                // esp8266 library
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

// required libraries
// Set these to run example.
#define FIREBASE_HOST "iot-home-security-6b9c2.firebaseio.com"         //please remove http and / from firebase url
#define FIREBASE_AUTH "kYRGhTzK2tfdjVYAkdX6KJBSK34EFvo8TWAFaWUk"      // paste secret key here
#define WIFI_SSID "Foxrine"
#define WIFI_PASSWORD "lamia2000"                                    //password of wifi ssid

String fireStatus = "";                                                     // led status received from firebase
int sig = 4;
int pir_led = 0;  
int pir_pin = 5;
int fb_led = 12;
int buzzer = 15;
int servoAngle; 
Servo servo; 
String myString; // sending integer data as string ,sensor data can also be send as integer
String motion = "Intruder Detected";   // string which stores state - motion/no motion
String nomotion= "No motion";
int sdata = 0; // PIR sensor  value will be stored in sdata.
 
  
void setup() {
  Serial.begin(115200);
  delay(1000);
//  Serial1.begin(9600);
  servo.attach(13);
  pinMode(sig,OUTPUT);
  digitalWrite(sig,LOW);
  pinMode(fb_led, OUTPUT);      
  pinMode(pir_led,OUTPUT);  
  pinMode(pir_pin, INPUT); // declare sensor as input
  pinMode(buzzer, OUTPUT);         

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  
  //Firebase.setString("IOT_Security_Panel/Security", fireStatus);
  servoAngle = Firebase.getInt("IOT_Security_Panel/servo");
  servo.write(servoAngle);
  
  
}

void loop() {
  Serial.println("Welcome to Shield Security Systems");
  servoAngle = Firebase.getInt("IOT_Security_Panel/servo");
  servo.write(servoAngle);
  fireStatus = Firebase.getString("IOT_Security_Panel/Security");                                      // get ld status input from firebase
  if (fireStatus == "1") {                                                          // compare the input of led status received from firebase
    Serial.println("System is active");                                                  // make bultin led ON
    digitalWrite(fb_led, HIGH);
    sdata = digitalRead(pir_pin);
    Serial.println("Searching for Intruder...");
    myString = String(sdata); 
    Serial.println(myString);//printing string to verify whether sensor is working or not    
    delay(1000);
    long state = digitalRead(pir_pin);

    if(state == HIGH) {
      
      digitalWrite (pir_led, HIGH);
      digitalWrite (buzzer, HIGH);
      Serial.println("Intruder detected!");
      Serial.println("Sending message..."); 
      digitalWrite(sig, HIGH);
//      Serial.println(state);
      Firebase.setString("IOT_Security_Panel/alert",motion);//A string will be sent to real time database under state- alert -ex -  motion , no motion 
      delay(2000);
    }
    
  } 
  else if (fireStatus == "0") {                                                  // compare the input of led status received from firebase
    Serial.println("System is deactivated");                                               // make bultin led OFF
    digitalWrite(fb_led, LOW);
    digitalWrite (pir_pin, LOW);
    digitalWrite(pir_led,LOW);
    digitalWrite (buzzer, LOW); 
    digitalWrite(sig, LOW);
    Serial.println("Sensor in Idle mode!");
    Firebase.setString("IOT_Security_Panel/alert",nomotion);
    delay(1000);
    
  }
  else {
    Serial.println("Wrong Credential! Please send ON/OFF");
    Serial.println(fireStatus);
  }

 
}
