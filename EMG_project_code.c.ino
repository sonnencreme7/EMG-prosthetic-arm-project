/#include <Servo.h>

// ===============================
// SERVO MOTORS
// ===============================

Servo servo_motor_1;
Servo servo_motor_2;
Servo servo_motor_3;
Servo servo_motor_4;
Servo servo_motor_5;

const int SERVO_PIN_1 = 9;
const int SERVO_PIN_2 = 10;
const int SERVO_PIN_3 = 11;
const int SERVO_PIN_4 = 6;
const int SERVO_PIN_5 = 5;

// ===============================
// EMG INPUT
// ===============================

const int EMG_PIN = A0;

float baseline = 0.0;
float smoothedEMG = 0.0;

// ===============================
// SMOOTHING
// ===============================

// Smaller alpha = smoother but slower
// Larger alpha = faster but noisier

const float alpha = 0.05;

// ===============================
// HYSTERESIS THRESHOLDS
// ===============================

// Adjust after observing the smoothed EMG

float thresholdON = 40.0;
float thresholdOFF = 20.0;

// ===============================
// SERVO POSITIONS
// ===============================

// Adjust for your mechanical design

const int OPEN_ANGLE = 0;
const int CLOSED_ANGLE = 90;

// ===============================
// HAND STATE
// ===============================

bool handClosed = false;

// ===============================
// SETUP
// ===============================

void setup() {

 Serial.begin(115200);

 servo_motor_1.attach(SERVO_PIN_1);
 servo_motor_2.attach(SERVO_PIN_2);
 servo_motor_3.attach(SERVO_PIN_3);
 servo_motor_4.attach(SERVO_PIN_4);
 servo_motor_5.attach(SERVO_PIN_5);

 servo_motor_1.write(OPEN_ANGLE);
 servo_motor_2.write(OPEN_ANGLE);
 servo_motor_3.write(OPEN_ANGLE);
 servo_motor_4.write(OPEN_ANGLE);
 servo_motor_5.write(OPEN_ANGLE);

 delay(1000);

 // Baseline calibration
 long sum = 0;
 const int samples = 1000;

 for (int i = 0; i < samples; i++) {
   sum += analogRead(EMG_PIN);
   delay(2);
 }

 baseline = sum / float(samples);
}

// ===============================
// MAIN LOOP
// ===============================

void loop() {

 // 1. Read EMG signal

 int raw = analogRead(EMG_PIN);

 // 2. Full-wave rectification

 float rectifiedEMG = abs(raw - baseline);

 // 3. Exponential smoothing

 smoothedEMG =
     alpha * rectifiedEMG
     + (1.0 - alpha) * smoothedEMG;

 // 4. Hysteresis control

 if (!handClosed && smoothedEMG > thresholdON) {

   handClosed = true;

   servo_motor_1.write(CLOSED_ANGLE);
   servo_motor_2.write(CLOSED_ANGLE);
   servo_motor_3.write(CLOSED_ANGLE);
   servo_motor_4.write(CLOSED_ANGLE);
   servo_motor_5.write(CLOSED_ANGLE);

 }

 else if (handClosed && smoothedEMG < thresholdOFF) {

   handClosed = false;

   servo_motor_1.write(OPEN_ANGLE);
   servo_motor_2.write(OPEN_ANGLE);
   servo_motor_3.write(OPEN_ANGLE);
   servo_motor_4.write(OPEN_ANGLE);
   servo_motor_5.write(OPEN_ANGLE);

 }

 // 5. Serial Plotter

 Serial.print("Raw:");
 Serial.print(raw);

 Serial.print(",Rectified:");
 Serial.print(rectifiedEMG);

 Serial.print(",Smoothed:");
 Serial.print(smoothedEMG);

 Serial.print(",ThresholdON:");
 Serial.print(thresholdON);

 Serial.print(",ThresholdOFF:");
 Serial.print(thresholdOFF);

 Serial.print(",HandClosed:");
 Serial.println(handClosed ? 1 : 0);

 delay(5);
}
