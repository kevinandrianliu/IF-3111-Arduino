/* Data */
int threshold[16] = {3, 34, 63, 91, 94, 120, 146, 167, 170, 192, 214, 233, 237, 255, 275, 291};
char keypad[16] = {'A', 'B', 'C', 'D', '3', '9', '6', '#', '2', '5', '8', '0', '1', '4', '7', '*'};

long duration;
float distance;

char selectedKeypad = 0x0;
char buzzerCount = 0x0;

/* Pins */
const int triggerPin = 9;
const int echoPin = 10;
const int buzzerPin = 11;

void operateKeypad();
void operateBuzzerByTimeFrequency(int miliseconds, int frequency);
void operateUltrasonicSensor();

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
    default:
      selectedKeypad = 0x0;
      break;
  }
  
  delay(500);
}

void operateKeypad(){
  // KEYPAD
  int value = analogRead(A0);
  for (int i = 0; i < 16; i++){
    if (abs(value - threshold[i] <= 1)){
      selectedKeypad = keypad[i];
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
