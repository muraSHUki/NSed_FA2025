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
const uint8_t IN3_PIN[] = { 4, 50, 12  ,  24, 28, 32  ,  36, 40, 44  ,  48};    // B, D, F  ,  H, J, L  ,  N, P, R  ,  T
const uint8_t IN4_PIN[] = { 5, 51, 13  ,  25, 29, 33  ,  37, 41, 45  ,  49};    // B, D, F  ,  H, J, L  ,  N, P, R  ,  T
// ----------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------
// ACTUATOR CONTROL SETUP
// ----------------------------------------------------------------------------------------------------
// --- Constants
// --- --- Power control for speed
const uint16_t EN_OFF  = 0;
const uint16_t EN_FULL = 4000;
const uint16_t DEADTIME_MS = 20;

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
  // for (int i=0; i<20; i++) moveActuator(i, 1);
  // delay(2000);

  // // Coast all A-F
  // for (int i=0; i<20; i++) moveActuator(i, 99);
  // delay(500);

  // // Retract all A-F
  // for (int i=0; i<20; i++) moveActuator(i, -1);
  // delay(2000);

  // // Final coast
  // for (int i=0; i<20; i++) moveActuator(i, 99);
  // --------------------------------------------------



  // --- Test 3 : Move all actuators sequentially (A–T)
  // --------------------------------------------------
  // for (int i = 0; i < 20; i++) {
  //   Serial.print("Testing actuator ");
  //   Serial.println(i);

  //   moveActuator(i, 1);   // extend
  //   delay(500);

  //   moveActuator(i, 99);  // coast / stop
  //   delay(100);

  //   moveActuator(i, -1);  // retract
  //   delay(500);

  //   moveActuator(i, 99);  // coast / stop again
  //   delay(100);
  // }
  // --------------------------------------------------
  // 1 3 4 8 11 13 14 15 16


  // --- Test 4 : Move specific acutator 
  // --------------------------------------------------
  // int i = 16;
  // moveActuator(i, 1); // extend F
  // delay(1000);
  // moveActuator(i, 99); // extend F
  // delay(500);
  // moveActuator(i, -1); // extend F
  // delay(1000);  
  // --------------------------------------------------



  // --- Test 5 : Test if second PCA board is speaking
  // --------------------------------------------------
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
  // --------------------------------------------------



  // --- Test 6 : two-phase 5×4 wave (rows 0–3, 4–7, 8–11, 12–15, 16–19) ---
  // -----------------------------------------------------------------------
  for (int i = 0; i < 20; i++) moveActuator(i, -1);
  delay(2000);
  for (int i = 0; i < 20; i++) moveActuator(i, 99);
  for (int i = 0; i < 20; i++) moveActuator(i, 1);
  delay(1000);

  // --- --- ROW DEFINITIONS
  const uint8_t ROW1[] = {0, 1, 2, 3};
  const uint8_t ROW2[] = {4, 5, 6, 7};
  const uint8_t ROW3[] = {8, 9, 10, 11};
  const uint8_t ROW4[] = {12, 13, 14, 15};
  const uint8_t ROW5[] = {16, 17, 18, 19};

  const uint8_t* ROWS[]   = {ROW1, ROW2, ROW3, ROW4, ROW5};
  const uint8_t ROW_SIZES[] = {4, 4, 4, 4, 4};
  const uint8_t NUM_ROWS = 5;

  // --- --- HELPER: move one entire row
  auto moveRow = [&](uint8_t rowIndex, int dir) {
    if (rowIndex >= NUM_ROWS) return;
    for (uint8_t j = 0; j < ROW_SIZES[rowIndex]; j++) {
      moveActuator(ROWS[rowIndex][j], dir);
    }
  };

  // --- --- PARAMETERS
  const int phase_period_ms = 100;   // time each phase lasts (lower = faster)
  const int cycles = 10;             // number of phase alternations

  // --- --- MAIN LOOP
  for (int c = 0; c < cycles; c++) {

    // ---------- PHASE 1 ----------
    // Odd rows (1,3,5 → 0,2,4 index) extend
    // Even rows (2,4 → 1,3 index) retract
    moveRow(0,  1);   // Row 1 up
    moveRow(2,  1);   // Row 3 up
    moveRow(4,  1);   // Row 5 up
    moveRow(1, -1);   // Row 2 down
    moveRow(3, -1);   // Row 4 down

    delay(phase_period_ms);

    // optional brief coast to avoid hard reversals
    for (int i = 0; i < 20; i++) moveActuator(i, 99);
    delay(30);

    // ---------- PHASE 2 ----------
    // Reverse directions
    moveRow(0, -1);   // Row 1 down
    moveRow(2, -1);   // Row 3 down
    moveRow(4, -1);   // Row 5 down
    moveRow(1,  1);   // Row 2 up
    moveRow(3,  1);   // Row 4 up

    delay(phase_period_ms);

    for (int i = 0; i < 20; i++) moveActuator(i, 99);
    delay(30);
  }

  // --- --- FINAL RETRACT / SHUTDOWN
  for (int i = 0; i < 20; i++) moveActuator(i, -1);
  delay(2000);
  for (int i = 0; i < 20; i++) moveActuator(i, 99);

  Serial.println("=== 2-phase 5×4 wave complete ===");
  // -----------------------------------------------------------------------






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