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

int motor_speed = 0;
uint8_t motor_max_speed = 500;

int main() {
  //myservo.attach(9);
  // Serial.begin(115200);

  TCCR1A |= (1<<COM1A1) | (1<<WGM11); // non-inverting mode for OC1A
  TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); // Mode 14, Prescaler 8

  ICR1 = 40000; // 320000 / 8 = 40000
  sei();
  DDRB |= (1<<1) | (1<<5);
  DDRD |= (1<<5) | (1<<6);

  DDRD &= 0xFF ^ (a_mask | b_mask);

  uint8_t val = ((PIND & a_mask) | (PIND & b_mask)) >> 2;

  // figure out where we booted
  for (uint8_t i = 0; i<4; i++) {
    if (val == spots[i]) {
      spot = i;
      break;
    }
  }

  uint8_t p, n;
  long diff;

  OCR1A = 3000;
  while(1) {
    val = (PIND & (a_mask | b_mask)) >> 2;
    p = spot > 0 ? spot-1 : 3;
    n = spot < 3 ? spot+1 : 0;

    if (val == spots[p]) {
      spot = p;
      position--;
    } else if (val == spots[n]) {
      spot = n;
      position++;
    }

    diff = position - targetPosition;

    if (abs(diff) % 1024 < 5) {
      PORTD |= (1<<5);
    } else {
      PORTD &= 0xFF ^ (1<<5);
    }

    if (abs(diff) < 5) {
      PORTB |= (1<<5);
      motor_speed = 0;
    } else {
      PORTB &= 0xFF ^ (1<<5);

      if (diff < 0) {
        motor_speed++;
        if (motor_speed > motor_max_speed) {
          motor_speed = motor_max_speed;
        }
      } else {
        motor_speed--;
        if (motor_speed < -motor_max_speed) {
          motor_speed = -motor_max_speed;
        }
      }
    }
    OCR1A = 3000 + motor_speed;
  }

  return 1;
}
