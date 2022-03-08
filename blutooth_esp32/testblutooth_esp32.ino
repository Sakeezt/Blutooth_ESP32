#include <BluetoothSerial.h>
BluetoothSerial btSerial;

#define BT_NAME "SAKEET_ESP32" // Set bluetooth name
#define BUILTIN_LED 2 // LED is active low

int motor1Pin1 = 25;    // Set pin for Motor 1 (LEFT)
int motor1Pin2 = 33;
int enable1Pin = 32;
int motor2Pin1 = 27;    // Set pin for Motor 2 (RIGHT)
int motor2Pin2 = 26;
int enable2Pin = 14;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle1 = 255;
int dutyCycle2 = 200;


boolean btConnected = false;
char key, previousKey;
int motorLeft, motorRight;
long previousMillis = 0;
int timeout = 1000;
void setup()
{
pinMode(BUILTIN_LED, OUTPUT);
pinMode(motor1Pin1, OUTPUT);
pinMode(motor1Pin2, OUTPUT);
pinMode(enable1Pin, OUTPUT);
pinMode(motor2Pin1, OUTPUT);
pinMode(motor2Pin2, OUTPUT);
pinMode(enable2Pin, OUTPUT);
// configure LED PWM functionalitites

ledcSetup(pwmChannel, freq, resolution);
// attach the channel to the GPIO to be controlled

ledcAttachPin(enable1Pin, pwmChannel);
ledcAttachPin(enable2Pin, pwmChannel);

ledcWrite(pwmChannel, dutyCycle1);
ledcWrite(pwmChannel, dutyCycle2);
Serial.begin(115200);
btSerial.begin(BT_NAME);
Serial.println("ESP32 Bluetooth Mobile Robot");
Serial.println();
digitalWrite(BUILTIN_LED, HIGH);
}
void forward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}


void backward(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void right(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
void left(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}
void stop(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}


void loop()
{
if (btSerial.available()) {
previousMillis = millis();
char inChar = (char)btSerial.read();
if (btConnected == false) {
btConnected = true;
digitalWrite(BUILTIN_LED, LOW); // Turn on led
Serial.println("Bluetooth connected.");
}
if (inChar >= '0' && inChar <= '9') {
key = inChar;
if (key != previousKey) {
switch (key) {
case '0':
Serial.println("Robot stop.");
stop();
break;
case '1':
Serial.println("Robot move forward.");
forward();
break;
case '2':
Serial.println("Robot move backward.");
backward();
break;
case '3':
Serial.println("Robot turn left.");
left();
break;
case '4':
Serial.println("Robot turn right.");
right();
break;
}
previousKey = key;
}
}
}
if (millis() - previousMillis > timeout &&
btConnected == true) {
Serial.println("Bluetooth disconnected.");
digitalWrite(BUILTIN_LED, HIGH); // Turn off led
btConnected = false;
}
}
