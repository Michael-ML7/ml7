// pins from AIRCON lighting
const int AIRCON_BLUE = 13;
const int AIRCON_RED = 12;
const int AIRCON_GREEN = 11;

// mineral lighting
#define MINERAL_RED A15
#define MINERAL_GREEN A14
#define MINERAL_BLUE A13

// pins from car general lighting
const int LIGHTING_RED = 10;
const int LIGHTING_GREEN = 9;
const int LIGHTING_BLUE = 8;

// pins for the fans
const int FAN1 = 48;
const int FAN2 = 47;
const float activitate_temperature = 30;
const float higher_activitate_temperature = 35;

// supersonic sensor
const int trig_pin = 53;
#define echo_pin A11
double duration, distance;

// pins for the colour sensor / read values
const int S0 = 28;
const int S1 = 29;
const int S2 = 30;
const int S3 = 31;
const int COLOUR_SENSOR_OUT = 4;
int colour_read;
int sensor_r, sensor_g, sensor_b;
int r_mx = 457, r_mn = 84, g_mx = 381, g_mn = 71, b_mx = 279, b_mn = 81;

// button for controlling the colour sensor
const int button = 22;
bool button_on = false;

// temperature sensor
#include "DHT.h"
#define temp_sensor_pin A0
#define DHTTYPE DHT11
float temperature, prev_temperature = -1;
DHT dht(temp_sensor_pin, DHTTYPE);

// XY joystick
#define joystick_x A2
#define joystick_y A3
int x_val, y_val;
// the x_val controls the forward/backward motion of the car
// the y_val controls the left/right motion of the car

// the front wheels
const int front_left = 44;
const int front_right = 45;
// the rear wheels
const int rear_left = 43;
const int rear_right = 49;

// servo
#include <Servo.h>
Servo myservo;
#define servo_pin A7

// buzzer
const int buzzer_pin = 2;
// buzzer warning signal freq = 1000

int temp; // some temp int for reading input

//clock_t T, NT;
//double get_time() {NT = clock() - T; return (double)(NT) / CLOCKS_PER_SEC * 1000;} // current milisecond

void setup() {
//  T = clock();
  // SETTING PIN MODES

  // aircon lighting
  pinMode(AIRCON_RED, OUTPUT);
  pinMode(AIRCON_BLUE, OUTPUT);
  pinMode(AIRCON_GREEN, OUTPUT);
//  digitalWrite(AIRCON_GREEN, HIGH);
  // general lighting
  pinMode(LIGHTING_RED, OUTPUT);
  pinMode(LIGHTING_BLUE, OUTPUT);
  pinMode(LIGHTING_GREEN, OUTPUT);
  digitalWrite(LIGHTING_RED, HIGH);
  digitalWrite(LIGHTING_BLUE, HIGH);
  digitalWrite(LIGHTING_GREEN, HIGH);
  // mineral lighting
  pinMode(MINERAL_RED, OUTPUT);
  pinMode(MINERAL_GREEN, OUTPUT);
  pinMode(MINERAL_BLUE, OUTPUT);
  // fan
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);
  // colour sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(COLOUR_SENSOR_OUT, INPUT);
  digitalWrite(S0, HIGH); // easier for the arduino to measure
  digitalWrite(S1, LOW); // easier for the arduino to measure
  // button for turning on the colour sensor
  pinMode(button, INPUT);
  // front wheel
  pinMode(front_left, OUTPUT);
  pinMode(front_right, OUTPUT);
  // rear wheel
  pinMode(rear_left, OUTPUT);
  pinMode(rear_right, OUTPUT);
  // temperautre sensor
  dht.begin();
  // servo
  myservo.attach(servo_pin);
  // buzzer
  pinMode(buzzer_pin, OUTPUT);
  // supersonic sensor
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);

  // some setups
  digitalWrite(AIRCON_RED, LOW);
  digitalWrite(AIRCON_BLUE, LOW);

  // turn on the terminal (Serial monitor)
  Serial.begin(9600);

  // testing place
//  digitalWrite(front_left, LOW);
//  digitalWrite(front_right, HIGH);
//  analogWrite(servo_pin, 200);
  myservo.write(80);
  delay(900);
  myservo.write(90);
}

int get_red () {
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  return pulseIn(COLOUR_SENSOR_OUT, digitalRead(COLOUR_SENSOR_OUT == HIGH ? LOW : HIGH));
}

int get_green () {
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  return pulseIn(COLOUR_SENSOR_OUT, digitalRead(COLOUR_SENSOR_OUT == HIGH ? LOW : HIGH));
}

int get_blue () {
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  return pulseIn(COLOUR_SENSOR_OUT, digitalRead(COLOUR_SENSOR_OUT == HIGH ? LOW : HIGH));
}

int curr_time;
bool flag = false;

void beep_beep_beep () {
  analogWrite(buzzer_pin, 1000);
  delay(10);
  digitalWrite(buzzer_pin, LOW);
  delay(90);
  analogWrite(buzzer_pin, 1000);
  delay(10);
  digitalWrite(buzzer_pin, LOW);
  delay(90);
  analogWrite(buzzer_pin, 1000);
  delay(10);
  digitalWrite(buzzer_pin, LOW);
}

void loop() {
  temp = digitalRead(button);
  if (temp == LOW) button_on = !button_on;
  if (button_on) {
    Serial.println("colour sensor working");
    sensor_r = get_red(); // inverse
    sensor_g = get_green(); // inverse
    sensor_b = get_blue(); // inverse
//    Serial.print("RED ");
//    Serial.print(sensor_r);
//    Serial.print(" BLUE ");
//    Serial.print(sensor_b);
//    Serial.print(" GREEN ");
//    Serial.println(sensor_g);
    if (sensor_r < sensor_b && sensor_r < sensor_g) {
      // red detected
      Serial.println("RED");
      analogWrite(MINERAL_RED, 255);
      analogWrite(MINERAL_GREEN, 0);
      analogWrite(MINERAL_BLUE, 0);

      // beep beep beep sound
      beep_beep_beep();
    }
    else if (sensor_b < sensor_r && sensor_b < sensor_g) {
      // blue detected
      Serial.println("BLUE");
      analogWrite(MINERAL_RED, 0);
      analogWrite(MINERAL_GREEN, 0);
      analogWrite(MINERAL_BLUE, 255);

      // beep beep beep sound
      beep_beep_beep();
    }
    else if (sensor_g < sensor_r && sensor_g < sensor_b) {
      // green detected
      Serial.println("GREEN");
      analogWrite(MINERAL_RED, 0);
      analogWrite(MINERAL_GREEN, 255);
      analogWrite(MINERAL_BLUE, 0);

      // beep beep beep sound
      beep_beep_beep();
    }
//    delay(1000);
    return;
  }

  x_val = analogRead(joystick_x);
  y_val = analogRead(joystick_y);
  Serial.print(x_val);
  Serial.print(" ");
  Serial.println(y_val);
  if (y_val > 1000 && x_val / 100 == 5) {
    // go forward
    digitalWrite(front_left, HIGH);
    digitalWrite(front_right, HIGH);
    digitalWrite(rear_left, HIGH);
    digitalWrite(rear_right, HIGH);
  }
  if (y_val < 10 && x_val / 100 == 5) {
    // go backward
    digitalWrite(front_left, LOW);
    digitalWrite(front_right, LOW);
    digitalWrite(rear_left, LOW);
    digitalWrite(rear_right, LOW);
  }
  if (x_val > 1000 && y_val / 100 == 5) {
    // left
    digitalWrite(front_left, LOW);
    digitalWrite(front_right, HIGH);
    digitalWrite(rear_left, LOW);
    digitalWrite(rear_right, HIGH);
  }
  if (x_val > 1000 && y_val / 100 == 5) {
    // right
    digitalWrite(front_left, HIGH);
    digitalWrite(front_right, LOW);
    digitalWrite(rear_left, HIGH);
    digitalWrite(rear_right, LOW);
  }
  digitalWrite(trig_pin, HIGH);
  delay(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  distance = duration * 0.34 / 2; // milimeter
//  Serial.println(distance);
  if (distance < 75) {
    digitalWrite(front_left, LOW);
    digitalWrite(front_right, LOW);
    digitalWrite(rear_left, LOW);
    digitalWrite(rear_right, LOW);
    analogWrite(buzzer_pin, 1000);
    delay(1000);
    digitalWrite(buzzer_pin, LOW);
  }
  
  temperature = dht.readTemperature();
//  Serial.println(temperature);
  temperature = 36;
  if (temperature > higher_activitate_temperature) {
    digitalWrite(FAN1, HIGH); digitalWrite(FAN2, HIGH);
    analogWrite(AIRCON_GREEN, 255);
    delay(10);
    digitalWrite(AIRCON_GREEN, LOW);
    delay(10);
    analogWrite(AIRCON_GREEN, 255);
    delay(10);
    digitalWrite(AIRCON_GREEN, LOW);
    delay(10);
    analogWrite(AIRCON_GREEN, 255);
  }
  else if (temperature > activitate_temperature) {
    digitalWrite(FAN1, HIGH); digitalWrite(FAN2, HIGH);
    analogWrite(AIRCON_GREEN, 100);
    delay(10);
    digitalWrite(AIRCON_GREEN, LOW);
    delay(10);
    analogWrite(AIRCON_GREEN, 100);
  }
  else {
    digitalWrite(FAN1, LOW); digitalWrite(FAN2, LOW);
    digitalWrite(AIRCON_GREEN, LOW);
  }
}
