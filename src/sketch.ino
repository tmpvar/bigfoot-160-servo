#include <Servo.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

Servo myservo;
Encoder myEnc(5, 6);

void setup()
{
  // myservo.attach(9);
  Serial.begin(115200);
}

int v = 0;
int dir = 1;

long oldPosition  = 0;
void loop() {
  // if (v>5) {
  //   dir = -1;
  // } else if (v < -5) {
  //   dir = 1;
  // }
  // v+=dir;
  // myservo.write(90+v);  // set servo to mid-point

  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }

//  delay(50);
}
