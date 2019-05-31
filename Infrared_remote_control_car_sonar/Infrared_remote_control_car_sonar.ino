//www.elegoo.com

#include <IRremote.h>
#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo
//------- IR REMOTE CODES ---------//
#define FORWARD 16736925
#define BACK    16754775
#define LEFT    16720605
#define RIGHT   16761405
#define STOP    16712445

#define RECV_PIN  12

#define ENB 5   // Left  wheel speed
#define IN1 7   // Left  wheel forward
#define IN2 8   // Left  wheel reverse
#define IN3 9   // Right wheel reverse
#define IN4 11  // Right wheel forward
#define ENA 6   // Right wheel speed
#define carSpeed 200  // initial speed of car >=0 to <=255

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long val;
unsigned long preMillis;
int middleDistance;
int Echo = A4;
int Trig = A5;
bool forward_active = false;


//Ultrasonic distance measurement Sub function
int getDistance() { //prendre le maximum de trois valeurs
  int value = 0;
  int resu = 0;
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);//10
  digitalWrite(Trig, LOW);
  value = (int)pulseIn(Echo, HIGH) / 58;
  //  if (value > 15) {
  resu = value;
  //  }
  //  else {
  //    resu = 444;
  //  }
  return resu;
}
void forward() {
  if  (middleDistance <= 25) {
    stop();
  }
  else {
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    forward_active = true;
    Serial.println("go forward!");
  }
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  forward_active = false;
  Serial.println("go back!");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  forward_active = false;
  Serial.println("go left!");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  forward_active = false;
  Serial.println("go right!");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("STOP!");
  forward_active = false;
}

void setup() {
  myservo.attach(3);  // attach servo on pin 3 to servo object
  myservo.write(85);  //setservo position according to scaled value
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
  irrecv.enableIRIn();
}

void loop() {
  middleDistance = getDistance();
  Serial.println(middleDistance);
  if (irrecv.decode(&results)) {
    preMillis = millis();
    val = results.value;
    Serial.println(val);
    irrecv.resume();
    switch (val) {
      case FORWARD: forward();    break;
      case BACK:    back();     break;
      case LEFT:    left();     break;
      case RIGHT:   right();    break;
      case STOP:    stop();     break;
      default:                  break;
    }
  }
  else {
    if (millis() - preMillis > 500) {
      stop();
      preMillis = millis();
    }
  }
  if  ((middleDistance <= 25) && (  forward_active == true)) {
    stop();
  }
}
