#ifndef PARKING_INDICATOR_H
#define PARKING_INDICATOR_H

#include "mbed.h"
#include "parking_info.hpp"
#include <vector>

class ParkingIndicator {
private:
  std::vector<DigitalOut> leds; // Vector to store DigitalOut objects for LEDs

public:
  ParkingIndicator(const std::vector<PinName> &pinNames) {
    for (auto pin : pinNames) {
      leds.push_back(DigitalOut(pin));
    }

    this->turnAllOn();
  }

    void update(const ParkingInfo &info) {
      // empty_indices = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; starts from 1.
      this->reset();
      for (JsonVariant i : info.empty_indices) {
        leds[i.as<int>() - 1] = 1;
        printf("updated spot: %d\n", i.as<int>() - 1);
      }
    }

  void reset() {
    for (auto &led : leds) {
      led = 0;
    }
  }

  void turnAllOn() { // all empty
    for (auto &led : leds) {
      led = 1;
    }
    // this->parkingInfo.parkedSpots = 0;
    // this->parkingInfo.emptySpots = this->parkingInfo.totalSpots;
  }

  void turnAllOff() { // all occupied
    for (auto &led : leds) {
      led = 0;
    }
    // this->parkingInfo.parkedSpots = this->parkingInfo.totalSpots;
    // this->parkingInfo.emptySpots = 0;
  }
};

#endif // PARKING_INDICATOR_H
