#ifndef PARKING_GATE_H
#define PARKING_GATE_H

#include "Servo.h"
#include "mbed.h"
#include "parking_info.hpp"
#include <cstdio>

#define OPEN_ANGLE 80.0f
#define CLOSE_ANGLE 0.0f

class ParkingGate {
private:
  Servo gateServo;
  float currentAngle;

public:
  ParkingGate(PinName pin) : gateServo(pin), currentAngle(CLOSE_ANGLE) {
    this->gateServo.write(CLOSE_ANGLE); // Start closed
  }

  void open() {
    this->gateServo.write(OPEN_ANGLE);
    this->currentAngle = OPEN_ANGLE;
    printf("Gate opened\n");
  }

  void close() {
    this->gateServo.write(CLOSE_ANGLE);
    this->currentAngle = CLOSE_ANGLE;
    printf("Gate closed\n");
  }

  void scanOpen() {
    // if (this->currentAngle == OPEN_ANGLE) return;

    for (float angle = CLOSE_ANGLE; angle <= OPEN_ANGLE; angle += 5.0f) {
      this->gateServo.write(angle);
      this->currentAngle = angle;
      thread_sleep_for(100);
    }
    printf("Gate gradually opened\n");
  }

  void scanClose() {
    // if (this->currentAngle == CLOSE_ANGLE) return;

    for (float angle = OPEN_ANGLE; angle >= CLOSE_ANGLE; angle -= 5.0f) {
      this->gateServo.write(angle);
      this->currentAngle = angle;
      thread_sleep_for(100);
    }
    printf("Gate gradually closed\n");
  }

  void update(const ParkingInfo &info) {
    printf("Total: %d, Parked: %d\n", info.totalSpots, info.parkedSpots);
    printf("Empty-indices: %d", info.empty_indices.size());
    if (info.empty_indices.size() == 0) {
      this->close();
    } else {
      this->open();
    }
  }
};

#endif // PARKING_GATE_H
