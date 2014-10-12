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
long position = 0, targetPosition = 0;

uint8_t spots[4] = {0, 1, 3, 2};
uint8_t spot = 0;


void idle() {
  analogWrite(5, 0);
  analogWrite(6, 0);
  digitalWrite(13, HIGH);
}

void exit_idle() {
  digitalWrite(13, LOW);
}

void fwd(uint8_t speed) {
  analogWrite(5, speed);
  analogWrite(6, 0);
  exit_idle();
}

void rev(uint8_t speed) {
  analogWrite(6, speed);
  analogWrite(5, 0);
  exit_idle();
}

void setup()
{
  // myservo.attach(9);
  Serial.begin(115200);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(13, OUTPUT);

  DDRD &= 0xFF ^ (a_mask | b_mask);

  uint8_t val = ((PIND & a_mask) | (PIND & b_mask)) >> 2;

  // figure out where we booted
  for (uint8_t i = 0; i<4; i++) {
    if (val == spots[i]) {
      spot = i;
      break;
    }
  }

  idle();
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
    time = millis();
  }

  if (position < targetPosition) {
    rev(127);
  } else if (position > targetPosition) {
    fwd(127);
  } else {
    idle();
  }
}
