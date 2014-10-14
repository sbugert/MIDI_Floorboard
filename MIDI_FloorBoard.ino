#define S0 11
#define S1 10
#define S2 9
#define Z 8

#define INPUTS 16

const int enablePins[] = {
  6, 7};

// =============================== User Settings ========================================

#define MIDICHANNEL 0    // MIDI channel: 0-15 for channels 1-16
#define VELOCITY 69
const unsigned char noteValue[INPUTS] = {
  60,62,64,65,
  67,69,71,72,
  74,76,77,79,
  81,83,84,86};  // array of midi note values

// ======================================================================================

//int expr = 0;

boolean activeSwitch[INPUTS] = {
  HIGH,HIGH,HIGH,HIGH,
  HIGH,HIGH,HIGH,HIGH,
  HIGH,HIGH,HIGH,HIGH,
  HIGH,HIGH,HIGH,HIGH};     // currently pressed toggles

void setup() {
  // Adress-Bits
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  pinMode(enablePins[0], OUTPUT);
  pinMode(enablePins[1], OUTPUT);

  digitalWrite(enablePins[0], HIGH);
  digitalWrite(enablePins[1], HIGH);

  pinMode(Z, INPUT);
  digitalWrite(Z, HIGH); // activate internal pull-up
  
//  analogReadResolution(8);

  Serial.begin(31250);
}

void loop() {
  for (int i = 0; i < INPUTS; i++) {
    if(readSwitch(i) == LOW && activeSwitch[i] == HIGH) {             // if toggle i pressed
      activeSwitch[i] = LOW;
      sendMIDI(144, noteValue[i], VELOCITY);    // note-on
      continue;
    }
    if(readSwitch(i) == HIGH && activeSwitch[i] == LOW) {
        activeSwitch[i] = HIGH;
        sendMIDI(128, noteValue[i], VELOCITY);    // note-off 128
    }
  }
 //     if(expr != map(analogRead(0), 0, 1023, 0, 127)) {
 //       expr = map(analogRead(0), 0, 1023, 0, 127);
 //       sendMIDI(176, 11, map(analogRead(0), 0, 1023, 0, 127));
 //     }
}

void sendMIDI(int cmd, int pitch, int velocity) {
  cmd = cmd + MIDICHANNEL;
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

int readSwitch(int input) {
  int multiplexerNr = input / 8;
  int port = input % 8;

  // Adresse setzen
  digitalWrite(S0, port & 0x1);
  digitalWrite(S1, (port >> 1) & 0x1);
  digitalWrite(S2, (port >> 2) & 0x1);

  // Enable auf low -> aktivieren
  digitalWrite(enablePins[multiplexerNr], LOW);
  delay(1);
  int in = digitalRead(Z);
  delay(1);
  digitalWrite(enablePins[multiplexerNr], HIGH);
  return in;
}
