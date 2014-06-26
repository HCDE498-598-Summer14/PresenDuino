#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX
int greenLed = 13;
int yellowLed = 12;
int redLed = 11;

char currentState = '0';

void setup()  
{
  pinMode(greenLed, OUTPUT); 
  pinMode(yellowLed, OUTPUT); 
  pinMode(redLed, OUTPUT); 
  
  digitalWrite(greenLed, HIGH); 
  digitalWrite(yellowLed, HIGH); 
  digitalWrite(redLed, HIGH);
  
  delay(1000);
  
  digitalWrite(greenLed, LOW); 
  digitalWrite(yellowLed, LOW); 
  digitalWrite(redLed, LOW);
  
   // Open serial communications
  Serial.begin(57600);
  mySerial.begin(19200);
}

void loop() // run over and over
{
  char recState;
  
  if( currentState == '4'){
          digitalWrite(yellowLed, HIGH);
          delay(150);
          digitalWrite(yellowLed, LOW);
          delay(150);
  }
  
  if (mySerial.available())
  {
   
    recState = mySerial.read();
    Serial.println("Current State:");
    Serial.println(currentState);
    Serial.println("Received State:");
    Serial.println(recState);
    
    if(recState != currentState){
      if(recState == '1'){
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
        currentState = recState;
        Serial.println("Changed state to '1'");
      }else if(recState == '4'){
        
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
        currentState = recState;
        Serial.println("Changed state to '4'");
      
      }else if(recState == '3'){
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, LOW);
        currentState = recState;
        Serial.println("Changed state to '3'");
      }
    }
  }  
}

