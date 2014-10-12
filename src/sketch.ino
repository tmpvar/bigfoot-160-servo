#include <Servo.h>

Servo myservo;

void setup()
{
  myservo.attach(9);
}

int v = 0;
int dir = 1;

void loop() {
  if (v>90) {
    dir = -1;
  } else if (v < -90) {
    dir = 1;
  }
  v+=dir;
  myservo.write(90+v);  // set servo to mid-point
  delay(50);
}
