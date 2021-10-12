#define CLK 2
#define DATA 3

#define PKTLEN 24

#define BTN_SELECT 7
#define BTN_START 8
#define BTN_R2 9
#define BTN_BLUE 10
#define BTN_L2 12
#define BTN_GREEN 13
#define BTN_YELLOW 14
#define BTN_RED 15
#define BTN_L1 17
#define BTN_R1 18
#define BTN_DOWN 19
#define BTN_UP 20
#define BTN_RIGHT 22
#define BTN_LEFT 23

void fillbuff(byte buff[]) {
  byte clk = 0;
  byte _clk = 0;
  for (byte b=0; b<48; b++) {
    while(true) {
      clk = ((PIND >> CLK) & 0x1);
      if (clk == 0 && _clk == 1) {
        buff[b] = ((PIND >> DATA) & 0x1);
        _clk = clk;
        break;
      }
      _clk = clk;
    }
  }
}

byte findstart(byte buff[]) {
  for (byte b=0; b<24; b++) {
    if (
      buff[b] == 0
      && buff[b+1] == 1
      && buff[b+2] == 1
      && buff[b+3] == 1
      && buff[b+4] == 1
      && buff[b+5] == 1
      && buff[b+6] == 0
      && buff[b+11] == 0
      && buff[b+16] == 0
      && buff[b+21] == 0
    ) {
     return b;
    }
  }
  return 255;
}

unsigned long getbtns(byte start, byte buff[], byte state[], byte _state[]) {
  bool changed = false;
  for (byte b=0; b<24; b++) {
    state[b] = buff[start+b];
    if (state[b] != _state[b]) {
      changed = true;
    }
  }
  return changed;
}

void printbtns(byte state[]) {
  for (byte b=0; b<24; b++) {
    Serial.print(state[b]);
  }
  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DATA, INPUT_PULLUP);
}

unsigned long clk = 0;
unsigned long _clk = 0;
byte buff[48];
byte state_a[24];
byte state_b[24];
bool s = false;

void loop() {
  fillbuff(buff);
  byte start = findstart(buff);
  if (start == 255) {
    Serial.print("BAD START");
    return;
  }
  bool changed;
  if (s) {
    changed = getbtns(start, buff, state_a, state_b);
  }
  else {
    changed = getbtns(start, buff, state_b, state_a);
  }
  if (changed) {
    if (s) {
      printbtns(state_a);
    }
    else {
      printbtns(state_b);
    }
    s = !s;
  }
}
