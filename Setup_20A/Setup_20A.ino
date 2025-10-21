// ----------------------------------------------------------------------------------------------------
// CODE FOR SETUP 20A
// --- as the name suggests this setup involves 20 acutators arragned in a 2x10 pattern
// --- last updated : 10/20/2025 --- 18:09
// ----------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------
// PREAMBLE
// ----------------------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// --- Instantiate two PCA boards for 20 actuators ---
Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);  // channels 0–15   (A–P)
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x41);  // channels 16–19  (Q–T)
// ----------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------
// CHANNEL DIRECTION
// ----------------------------------------------------------------------------------------------------
// --- PCA9685 channels for EN pins
// --- --- Shows the wiring control from the PCA board to each LN board
const uint8_t EN_CH[] = { 
  0   , 1   ,   // LN1  : A, B
  2   , 3   ,   // LN2  : C, D
  4   , 5   ,   // LN3  : E, F
  6   , 7   ,   // LN4  : G, H
  8   , 9   ,   // LN5  : I, J
  10  , 11  ,   // LN6  : K, L
  12  , 13  ,   // LN7  : M, N
  14  , 15  ,   // LN8  : O, P
  16  , 17  ,   // LN9  : Q, R
  18  , 19  ,   // LN10 : S, T
};

// --- Arduino pins for IN1/IN2 per actuator (direction control)
// --- --- Shows the Arduino pin connections to each of the LN boards
// --- --- --- Counting numbers and letters because skill issue
// --- --- --- A B C D E F G H I  J  K  L  M  N  O  P  Q  R  S  T
// --- --- --- 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
const uint8_t IN1_PIN[] = { 2, 6, 10  ,  22, 26, 30  ,  34, 38, 42  ,  46};    // A, C, E  ,  G, I, K  ,  M, O, Q  ,  S 
const uint8_t IN2_PIN[] = { 3, 7, 11  ,  23, 27, 31  ,  35, 39, 43  ,  47};    // A, C, E  ,  G, I, K  ,  M, O, Q  ,  S
const uint8_t IN3_PIN[] = { 4, 8, 12  ,  24, 28, 32  ,  36, 40, 44  ,  48};    // B, D, F  ,  H, J, L  ,  N, P, R  ,  T
const uint8_t IN4_PIN[] = { 5, 9, 13  ,  25, 29, 33  ,  37, 41, 45  ,  49};    // B, D, F  ,  H, J, L  ,  N, P, R  ,  T
// ----------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------
// ACTUATOR CONTROL SETUP
// ----------------------------------------------------------------------------------------------------
// --- Constants
// --- --- Power control for speed
const uint16_t EN_OFF  = 0;
const uint16_t EN_FULL = 2600;
const uint16_t DEADTIME_MS = 120;

// --- Helpers
// --- --- Force PCA channel
static inline void setEN(uint8_t ch, uint16_t duty) {
  if (duty > 4095) duty = 4095;

  // Route to the correct PCA board based on channel index
  if (ch < 16) {
    pca1.setPWM(ch, 0, duty);
  } else {
    pca2.setPWM(ch - 16, 0, duty);
  }
}


// --- Define movement
// --- --- Nuf said
void moveActuator(uint8_t index, int dir) {
  uint8_t en_ch = EN_CH[index];

  uint8_t in1, in2;
  if (index % 2 == 0) {       // define that even index: uses IN1/IN2
    in1 = IN1_PIN[index/2];
    in2 = IN2_PIN[index/2];
  } else {                    // define that odd index: uses IN3/IN4
    in1 = IN3_PIN[index/2];
    in2 = IN4_PIN[index/2];
  }

  if (dir == 1) {             // define extend
    digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
    delay(DEADTIME_MS);
    setEN(en_ch, EN_FULL);
  } else if (dir == -1) {     // define retract
    digitalWrite(in1, LOW);  digitalWrite(in2, HIGH);
    delay(DEADTIME_MS);
    setEN(en_ch, EN_FULL);
  } else if (dir == 0) {      // define brake
    digitalWrite(in1, HIGH); digitalWrite(in2, HIGH);
    setEN(en_ch, EN_FULL);
  } else if (dir == 99) {     // define coast
    setEN(en_ch, EN_OFF);
  }
}
// ----------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------
// DEFINE MOVEMENT
// ----------------------------------------------------------------------------------------------------
// --- Guide
//        moveActuator(index, dir)
//          index : 0..19 (A=0, B=1, ... , T=20)
//          dir   :  1 = extend
//                  -1 = retract
//                   0 = brake
//                  99 = coast

//        to move multiple actuators simultaniously
//          for ( i = *index of actuators* ) moveActuator(i, dir)
// --- Movement
// --- --- Keep this preamble
// --- --- --- From here
// -----------------------------------
void setup() {
  // Direction pins
  for (uint8_t i=0; i<4; i++) {
    pinMode(IN1_PIN[i], OUTPUT);
    pinMode(IN2_PIN[i], OUTPUT);
    pinMode(IN3_PIN[i], OUTPUT);
    pinMode(IN4_PIN[i], OUTPUT);
  }

  // Safe idle
  for (uint8_t i=0; i<4; i++) {
    digitalWrite(IN1_PIN[i], LOW);
    digitalWrite(IN2_PIN[i], LOW);
    digitalWrite(IN3_PIN[i], LOW);
    digitalWrite(IN4_PIN[i], LOW);
  }

  Wire.begin();
  // --- Initialize both PCA boards ---
  pca1.begin();
  pca2.begin();
  pca1.setPWMFreq(1000);
  pca2.setPWMFreq(1000);

// --- Optional serial diagnostics ---
Serial.begin(115200);
Serial.println("Both PCA boards initialized (0x40, 0x41)");

  // -----------------------------------
  // --- --- --- To here

  // -----------------------------------
  // Can define movement from here down 
  // -----------------------------------



  // --- Test 1 : Make sure actuators A-F work
  // --------------------------------------------------
  // moveActuator(0, 1); // extend A
  // delay(2000);
  // moveActuator(0, 99); // wait A
  // delay(500);
  // moveActuator(0, -1); // retract A
  // delay(2000);

  // delay(1000);

  // moveActuator(1, 1); // extend B
  // delay(2000);
  // moveActuator(1, 99); // wait B
  // delay(500);
  // moveActuator(1, -1); // retract B
  // delay(2000);

  // delay(1000);

  // moveActuator(2, 1); // extend C
  // delay(2000);
  // moveActuator(2, 99); // wait C
  // delay(500);
  // moveActuator(2, -1); // retract C
  // delay(2000);

  // delay(1000);

  // moveActuator(3, 1); // extend D
  // delay(2000);
  // moveActuator(3, 99); // wait D
  // delay(500);
  // moveActuator(3, -1); // retract D
  // delay(2000);

  // delay(1000);

  // moveActuator(4, 1); // extend E
  // delay(2000);
  // moveActuator(4, 99); // wait E
  // delay(500);
  // moveActuator(4, -1); // retract E
  // delay(2000);

  // delay(1000);

  // moveActuator(5, 1); // extend F
  // delay(2000);
  // moveActuator(5, 99); // extend F
  // delay(500);
  // moveActuator(5, -1); // extend F
  // delay(2000);  
  // --------------------------------------------------

  // --- Test 2 : Move actuators A-F simultaniously
  // --------------------------------------------------
  // Extend all A-F
  // for (int i=0; i<6; i++) moveActuator(i, 1);
  // delay(2000);

  // // Coast all A-F
  // for (int i=0; i<6; i++) moveActuator(i, 99);
  // delay(500);

  // // Retract all A-F
  // for (int i=0; i<6; i++) moveActuator(i, -1);
  // delay(2000);

  // // Final coast
  // for (int i=0; i<6; i++) moveActuator(i, 99);
  // --------------------------------------------------

  // --- Test all actuators sequentially (A–T)
  // for (int i = 0; i < 20; i++) {
  //   Serial.print("Testing actuator ");
  //   Serial.println(i);

  //   moveActuator(i, 1);   // extend
  //   delay(2000);

  //   moveActuator(i, 99);  // coast / stop
  //   delay(500);

  //   moveActuator(i, -1);  // retract
  //   delay(2000);

  //   moveActuator(i, 99);  // coast / stop again
  //   delay(500);

  int i = 0;
  moveActuator(i, 1); // extend F
  delay(1000);
  moveActuator(i, 99); // extend F
  delay(500);
  moveActuator(i, -1); // extend F
  delay(1000);  

  // 2 3 16

  // --- Diagnostic test: blink each PCA board separately ---
  // Serial.println("=== Diagnostic: PCA board outputs ===");

  // // Board 1 (0x40) channel 0 ON
  // Serial.println("PCA #1 (0x40) channel 0 ON");
  // pca1.setPWM(0, 0, 3000);
  // delay(2000);
  // pca1.setPWM(0, 0, 0);

  // // Board 2 (0x41) channel 0 ON
  // Serial.println("PCA #2 (0x41) channel 0 ON");
  // pca2.setPWM(0, 0, 3000);
  // delay(2000);
  // pca2.setPWM(0, 0, 0);




  // Keep what's below this so actuators stop when done
  // --- Else they'll go on forever
  while (true) { delay(1000); } // halt forever
}
// End of movement


// Stop everything
// --- Make sure nothing keeps moving
void loop() {
  // not used
}
// ----------------------------------------------------------------------------------------------------