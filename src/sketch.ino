// #include <Servo.h>
// Servo myservo;

int v = 0;
int dir = 1;
uint8_t a, b, la, lb;

uint8_t a_mask = (1 << 2);
uint8_t b_mask = (1 << 3);

// A B
// 0 0
// 0 1
// 1 1
// 1 0

unsigned long time = 0;
long position = 0;

uint8_t spots[4] = {0, 1, 3, 2};
uint8_t spot = 0;


void setup()
{
  // myservo.attach(9);
  Serial.begin(115200);
  DDRD &= 0xFF ^ (a_mask | b_mask);

  uint8_t val = ((PIND & a_mask) | (PIND & b_mask)) >> 2;

  // figure out where we booted
  for (uint8_t i = 0; i<4; i++) {
    if (val == spots[i]) {
      spot = i;
      break;
    }
  }
}


void loop() {
  uint8_t val = ((PIND & a_mask) | (PIND & b_mask)) >> 2;
  uint8_t p = spot > 0 ? spot-1 : 3;
  uint8_t n = spot < 3 ? spot+1 : 0;

  unsigned long now = millis();

  if (val == spots[p]) {
    time = now;
    spot = p;
    position--;
  } else if (val == spots[n]) {
    spot = n;
    position++;
    time = now;
  } else if (now - time > 1000) {
    Serial.println(position);
    time = now;
  }
}
