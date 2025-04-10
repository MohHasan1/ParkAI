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

  //   void update(const ParkingInfo &info) {
  //     // empty_indices = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; starts from 1.
  //     this->reset();
  //     for (JsonVariant i : info.empty_indices) {
  //       leds[i.as<int>() - 1] = 1;
  //       printf("updated spot: %d\n", i.as<int>() - 1);
  //     }
  //   }

  void update(const ParkingInfo &info) {
    this->reset();
    if (info.empty_indices.size() == 0) {
      printf("No empty spots available\n");
      return;
    }

    for (JsonVariant i : info.empty_indices) {
      int index = i.as<int>() - 1; // Convert 1-based to 0-based index
      if (index >= 0 && index < leds.size()) {
        leds[index] = 1; // Turn on the LED for the empty spot
        printf("updated spot: %d\n", index);
      } else {
        printf("Invalid index: %d\n", index);
      }
    }
  }

  //   void reset() {
  //     for (auto &led : leds) {
  //       led = 0;
  //     }
  //   }

  void reset() {
    bool allOff = true;
    for (auto &led : leds) {
      if (led) {
        allOff = false;
        break;
      }
    }
    if (!allOff) {
      for (auto &led : leds) {
        led = 0;
      }
    }
  }

  //   void turnAllOn() { // all empty
  //     for (auto &led : leds) {
  //       led = 1;
  //     }
  //   }

  void turnAllOn() {
    bool allOn = true;
    for (auto &led : leds) {
      if (!led) {
        allOn = false;
        break;
      }
    }
    if (!allOn) {
      for (auto &led : leds) {
        led = 1;
      }
    }
  }

  void turnAllOff() { // all occupied
    for (auto &led : leds) {
      led = 0;
    }
  }
};

#endif // PARKING_INDICATOR_H
