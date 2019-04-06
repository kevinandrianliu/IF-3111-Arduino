int threshold[16] = {3, 34, 63, 91, 94, 120, 146, 167, 170, 192, 214, 233, 237, 255, 275, 291};
char keypad[16] = {'A', 'B', 'C', 'D', '3', '9', '6', '#', '2', '5', '8', '0', '1', '4', '7', '*'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(A0);
  for (int i = 0; i < 16; i++){
    if (abs(value - threshold[i] <= 1)){
      Serial.println(keypad[i]);

      break;
    }
  }
//  Serial.println(value);
  delay(400);
}
