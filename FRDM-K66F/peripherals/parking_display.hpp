#ifndef PARKING_DISPLAY_H
#define PARKING_DISPLAY_H

#include "LCDi2c.h"
#include "mbed.h"
#include "parking_info.hpp"
#include <cstdio>
#include <string>
#include <vector>

class ParkingDisplay {
private:
  LCDi2c lcd;

public:
  ParkingDisplay(PinName sda, PinName scl) : lcd(sda, scl, LCD16x2) { lcd.cls(); }

  void showWelcomeMessage() {
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Welcome");
    ThisThread::sleep_for(2000ms);
  }

  void update(const ParkingInfo &info) {
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Empty spots:");

    lcd.locate(0, 1);
    if (info.empty_indices.size() == 0) {
      lcd.printf("Sorry, No Spots!");
      return;
    }

    char buffer[17] = {0};
    int offset = 0;

    for (size_t i = 0; i < info.empty_indices.size(); ++i) {
      int written = snprintf(buffer + offset, sizeof(buffer) - offset, (i == 0) ? "%d" : ",%d", info.empty_indices[i].as<int>());
      if (written <= 0 || offset + written >= static_cast<int>(sizeof(buffer))) {
        break;
      }
      offset += written;
    }

    lcd.printf("%s", buffer);
  }
};

#endif // LCD_DISPLAY_H