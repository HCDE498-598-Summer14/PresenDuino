/*
PresenDuino assists in clocking time while you're presenting. It sends the current 
state to SignalSide, which lights up the LEDs respectively.
Uses:
  Arduino UNO
  Xbee series 1
  2 Switches
  LCD
*/

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
// for Xbee, RX of the Arduino connects to TX of the Xbee and vice versa
SoftwareSerial xbeeSerial(8, 9); // RX, TX

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

int redLed = 8;
int yellowLed = 9;
int greenLed = 10;

volatile int timeSec = 0;
volatile int countDownState = LOW;
int maxTimeSec = 0;
int timeSecDone = 0;

void setup() {
  // create a new character
  lcd.createChar(0, heart);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(1, 0);
  lcd.print("Made with ");
  lcd.write(byte(0));
  lcd.print(" in");
  lcd.setCursor(5, 1);
  lcd.print("HCDE DL");

  // initialize the digital pin as an output.
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT); 
  pinMode(greenLed, OUTPUT);
  
  digitalWrite(redLed, HIGH);
  digitalWrite(yellowLed, HIGH);
  digitalWrite(greenLed, HIGH);
  
  //In UNO int.0 is at pin 2, and int.1 is at pin 3
  attachInterrupt(0, changeCountDownState, RISING);
  attachInterrupt(1, incTime, RISING);
  
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // begin the serial connection with xbee
  xbeeSerial.begin(19200);
  
  // Delay in mil sec, so that we can read
  delay(2500);
  
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
}

void loop() {
  // Clear the LCD screen and position the cursor in upper-left corner
  lcd.clear();
  lcd.print(timeSec/60);
  lcd.print(" min, ");
  lcd.print(timeSec%60);
  lcd.print(" sec");
  
  if(maxTimeSec < timeSec + timeSecDone){
    maxTimeSec = timeSec + timeSecDone;
  }
  
  if(countDownState == HIGH){
    if(timeSec > 0){
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      
      if(timeSec < 10){
        // send to xbee serial connection
        xbeeSerial.println('4');
        Serial.println("Sent '4' to Signal Side");
        
        for(int i=0; i<3; i++){
          digitalWrite(yellowLed, HIGH);
          delay(150);
          digitalWrite(yellowLed, LOW);
          delay(150);
        }
        
      }else{
        // send to xbee serial connection
        xbeeSerial.println('1');
        Serial.println("Sent '1' to Signal Side");
        delay(900);
      }
      timeSec--;
      timeSecDone++;
    }else{
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      
      // send to xbee serial connection
      xbeeSerial.println('3');
      Serial.println("Sent '3' to Signal Side");
      
      countDownState = LOW;
      timeSec = maxTimeSec;
      timeSecDone = 0;
    }
  }else{
      // send to xbee serial connection
      xbeeSerial.println('3');
      Serial.println("Sent '3' to Signal Side");
  }
  delay(100);
}

void incTime(){
  timeSec = timeSec + 15;
}

void changeCountDownState(){
  countDownState = !countDownState;
}
