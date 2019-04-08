#include <LiquidCrystal.h>

/* Data */
// KEYPAD DATA
int threshold[16] = {14, 46, 74, 95, 97, 131, 155, 170, 172, 201, 223, 234, 236, 260, 278, 296};
char keypad[16] = {'A', 'B', 'C', 'D', '3', '6', '9', '#', '2', '5', '8', '0', '1', '4', '7', '*'};

// ULTRASONIC SENSOR DATA
long duration;
float distance;

char selectedKeypad = 0x0;
char buzzerCount = 0x0;
int timesAlarmTriggered = 0;

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
void getString();

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
}

void loop() {
  // put your main code here, to run repeatedly:
  operateKeypad();
  
  switch (selectedKeypad){
    case 'A': // Arm alarm
      buzzerCount = 0x0;
      operateUltrasonicSensor();
      break;
    case 'B':
      buzzerCount = 0x0;
      selectedKeypad = 0x0;
      break;
    case 'C':
      buzzerCount = 0x0;
      selectedKeypad = 0x0;
      break;
    case 'D': // Test alarm
      buzzerCount++;
      
      if (buzzerCount == 0x1){
        Serial.println("Press once more to test");
      } else if (buzzerCount == 0x2){
        Serial.println("Press any key to turn off");
        // ADD BUZZER SOUND
      } else if (buzzerCount == 0x3){
        Serial.println("Testing done");
        buzzerCount = 0x0;
      }
      selectedKeypad = 0x0;
      break;
  }

  operateSevenSegment(timesAlarmTriggered);
  delay(500);
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
}

void operateSevenSegment(int selection){
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,segmentArray[selection]);
  digitalWrite(latchPin,HIGH);
}

void getString(){
  int i = 0;
  selectedKeypad = 0x0;
  lcd.setCursor(0,1);
  while (i < 5){
    operateKeypad();
    if (selectedKeypad == 0x0){
      i++;
    } else {
      lcd.print(selectedKeypad);
    }
  }
}
