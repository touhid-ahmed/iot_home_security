int sys_led = 13;                // the pin that the LED is atteched to
int val = 0; 
int gsm = 0;


void SendMessage()
{
  Serial.println("Signal recieved from NodeMCU"); 
  Serial.println ("Sending Message...");
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println("AT+CMGS=\"01745684699\""); //Mobile phone number to send message
  delay(1000);
  Serial.println("An intruder has been detected at your house!");// Messsage content
  delay(1000);
  Serial.write(26);
}
 
  void setup() {
  pinMode(sys_led, OUTPUT);      // initalize LED as an output
  pinMode (A0, INPUT);
  Serial.begin(9600);        // initialize serial
}

void loop(){
int val = analogRead(A0);
int val1 = map(val, 0, 1024, 0, 255);
int val2 = (val1*5)/255;
//Serial.println(val2);
if (val2 ==1) {           // check if the sensor is HIGH
      digitalWrite(sys_led, HIGH);
      //Serial.println("Signal recieved from NodeMCU");
      gsm = gsm + 1;
      if (gsm == 1){
        SendMessage();
        delay(1000);
        
      }
    }
  
else {
      digitalWrite(sys_led, LOW); // turn LED OFF
      delay(500);             // delay 200 milliseconds 
      gsm=0;
      val2=0;
      
  }
}
