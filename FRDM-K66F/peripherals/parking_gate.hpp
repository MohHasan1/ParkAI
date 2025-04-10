#ifndef PARKING_GATE_H
#define PARKING_GATE_H

#include "Servo.h"
#include "mbed.h"
#include "parking_info.hpp"
#include <cstdio>

#define CLOSE_ANGLE 40.0f
#define OPEN_ANGLE 180.0f
#define MAX_SERVO_ANGLE 180.0f 

class ParkingGate {
private:
  Servo gateServo;
  float currentAngle;

  void writeAngle(float angle) {
    float percent = angle / MAX_SERVO_ANGLE; // normalize to 0.0 - 1.0
    gateServo.write(percent);
    currentAngle = angle;
  }

public:
  ParkingGate(PinName pin) : gateServo(pin), currentAngle(CLOSE_ANGLE) {
    writeAngle(CLOSE_ANGLE); // Start closed
  }

  void open() {
    writeAngle(OPEN_ANGLE);
    printf("Gate opened\n");
  }

  void close() {
    writeAngle(CLOSE_ANGLE);
    printf("Gate closed\n");
  }

  void scanOpen() {
    for (float angle = CLOSE_ANGLE; angle <= OPEN_ANGLE; angle += 5.0f) {
      writeAngle(angle);
      thread_sleep_for(100);
    }
    printf("Gate gradually opened\n");
  }

  void scanClose() {
    for (float angle = OPEN_ANGLE; angle >= CLOSE_ANGLE; angle -= 5.0f) {
      writeAngle(angle);
      thread_sleep_for(100);
    }
    printf("Gate gradually closed\n");
  }

  void loop() {
    for (int i = 0; i < 100; i++) {
      gateServo = i / MAX_SERVO_ANGLE;
      thread_sleep_for(100);
    }
    for (int i = 100; i > 0; i--) {
      gateServo = i / 100.0;
      thread_sleep_for(100);
    }
  }

  void update(const ParkingInfo &info) {
    printf("Total: %d, Parked: %d\n", info.totalSpots, info.parkedSpots);
    printf("Empty-indices: %d\n", (int)info.empty_indices.size());

    if (info.empty_indices.size() == 0) {
      close();
    } else {
      open();
    }
  }
};

#endif // PARKING_GATE_H
