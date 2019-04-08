#include <LiquidCrystal.h>

/* Data */
// KEYPAD DATA
int threshold[16] = {32, 44, 72, 98, 107, 130, 154, 173, 175, 196, 217, 236, 239, 263, 274, 296};
char keypad[16] = {'A', 'B', 'C', 'D', '3', '6', '9', '#', '2', '5', '8', '0', '1', '4', '7', '*'};

// ULTRASONIC SENSOR DATA
long duration;
float distance;

char selectedKeypad = 0x0;
char buzzerCount = 0x0;
int timesAlarmTriggered = 0;
char password[16] = {0x0};
char savedPassword[16] = {0x0};

char condition;
// Conditions = 0 Off, 1 Armed, 2 Alert, 3 Test
char passTries = 3;

// 7 SEGMENT DISPLAY DATA
int segmentArray[16] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156, 122, 158, 142};

// LCD DATA
const int rs = 7, e = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

/* Pins */
// ULTRASONIC SENSOR PINS
const int triggerPin = 9;
const int echoPin = 10;

// BUZZER PINS
const int buzzerPin = 11;

// 7 SEGMENT DISPLAY PINS
const int latchPin = 12;
const int clockPin = 8;
const int dataPin = 13;

void operateKeypad();
void operateBuzzerByTimeFrequency(int miliseconds, int frequency);
void operateUltrasonicSensor();
bool getPassword();

void setup() {
  // put your setup code here, to run once:
  // Setting up ultrasonic sensor pins
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Setting up 7 segment display pins
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  // Setting up LCD Display
  lcd.begin(16,2);
  Serial.begin(9600);

  condition = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  operateKeypad();
  switch(condition){
    case 0:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OFF");
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ARMED");
      lcd.setCursor(0,1);
      switch (passTries){
        case 3:
          lcd.print("Tries: 3");
          break;
        case 2:
          lcd.print("Tries: 2");
          break;
        case 1:
          lcd.print("Tries: 1");
          break;
      }
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ALERT");
      delay(300);
      lcd.clear();
      delay(300);
      lcd.setCursor(0,0);
      lcd.print("ALERT");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("TESTING");
      lcd.setCursor(0,1);
      lcd.print("Press D to off..");
      break;
    default:
      break;
  }
  switch (selectedKeypad){
    case 'A': // Arm alarm
    {
      buzzerCount = 0x0;
      
      bool stat = getPassword();
      if (stat){
        condition = 1;
        operateBuzzerByTimeFrequency(100, 2500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alarm armed");
        delay(3000);
        for (int i = 0; i < 16; i++){
          savedPassword[i] = password[i];
          password[i] = 0x0;
        }
      } else {
        for (int i = 0; i < 16; i++){
          password[i] = 0x0;
        }
      }
      break;
    }
    case 'B': // Deactivate alarm
    {
      if ((condition == 1) || (condition == 2)){
        bool stat = getPassword();
        if (stat){
          bool same = true;
          for (int i = 0; i < 16; i++){
            if (password[i] != savedPassword[i]){
              same = false;
              break;
            }
          }
          if (same) {
            condition = 0;
            for (int i = 0; i < 16; i++){
              savedPassword[i] = 0x0;
              password[i] = 0x0;
            }
            operateBuzzerByTimeFrequency(100, 2500);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Alarm disarmed");
            delay(3000);
            passTries = 0;
          } else {
            passTries++;
            operateBuzzerByTimeFrequency(200, 1000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Pass mismatch");
            delay(300);
            lcd.clear();
            delay(300);
            lcd.setCursor(0,0);
            lcd.print("Pass mismatch");
            delay(300);
            lcd.clear();
            delay(300);

            if (passTries > 2){
              condition = 2;
            }
          }
        } else {
          for (int i = 0; i < 16; i++){
            password[i] = 0x0;
          }
        }
      } else {
        operateBuzzerByTimeFrequency(200, 1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alarm not armed");
        delay(300);
        lcd.clear();
        delay(300);
        lcd.setCursor(0,0);
        lcd.print("Alarm not armed");
        
      }
      break;
    }
    case 'D': // Test alarm
    {
      if ((condition == 0) || (condition == 3)){
        buzzerCount++;
      
        if (buzzerCount == 0x1){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Press again");
          lcd.setCursor(0,1);
          lcd.print("to test...");
          delay(1000);
        } else if (buzzerCount == 0x2){
          condition = 3;
        } else if (buzzerCount == 0x3){
          condition = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Testing done");
          delay(1000);
          buzzerCount = 0x0;
        }
        selectedKeypad = 0x0;
        break;
      }
    }
    default:
      buzzerCount = 0x0;
  }
  
  if (condition == 1){
    operateUltrasonicSensor();
  } else if (condition == 2){
    operateBuzzerByTimeFrequency(800, 2000);
  } else if (condition == 3){
    operateBuzzerByTimeFrequency(800, 2000);
  }
  operateSevenSegment(timesAlarmTriggered);
  delay(500);
  selectedKeypad = 0x0;
}

void operateKeypad(){
  // KEYPAD
  int value = analogRead(A0);

  for (int i = 0; i < 16; i++){
    if (abs(value - threshold[i] <= 1)){
      selectedKeypad = keypad[i];
      Serial.println(keypad[i]);
      operateBuzzerByTimeFrequency(80, 1000);
      break;
    }
  }
  if (condition == 2){
    operateBuzzerByTimeFrequency(800, 2000);
  } 
}

void operateBuzzerByTimeFrequency(int miliseconds, int frequency){
    // BUZZER (very loud)
    tone(buzzerPin, frequency);
    delay(miliseconds);
    noTone(buzzerPin);
}

void operateUltrasonicSensor(){
  // ULTRASONIC SENSOR
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 35.00){
    condition = 2;
    timesAlarmTriggered++;
  }
}

void operateSevenSegment(int selection){
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,segmentArray[selection]);
  digitalWrite(latchPin,HIGH);
}

bool getPassword(){
  int i = 5;
  int charCount = 0;
  selectedKeypad = 0x0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter pass");
  delay(200);
  while (i > 0){
    operateKeypad();
    lcd.setCursor(15,0);
    lcd.print(i);
    lcd.setCursor(charCount,1);
    if (selectedKeypad == 0x0){
      i--;
    } else if ((selectedKeypad == '#') && (charCount > 0)) {
      break;
    } else {
      if (charCount < 16){
        lcd.print(selectedKeypad);
        password[charCount] = selectedKeypad;
      }
      charCount++;
      i = 5;
    }

    selectedKeypad = 0x0;
    delay(1000);
  }

  if (i > 0){
    return true;
  } else {
    operateBuzzerByTimeFrequency(200, 1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("No pass set");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0,0);
    lcd.print("No pass set");
    return false;
  }
  lcd.clear();
  delay(2000);
}
