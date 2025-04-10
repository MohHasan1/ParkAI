#ifndef ERROR_INDICATOR_H
#define ERROR_INDICATOR_H

#include "mbed.h"

class ErrorIndicator {
private:
  DigitalOut errorLed;

public:
  ErrorIndicator(PinName ledPin) : errorLed(ledPin) {
    errorLed = 0; // Start with LED off
  }

  void signalError() {
    errorLed = 1; // Turn on LED to indicate error
  }

  void clearError() {
    errorLed = 0; // Turn off LED
  }

  void blinkError(int times, std::chrono::milliseconds delay = 250ms) {
    for (int i = 0; i < times; ++i) {
      errorLed = 1;
      ThisThread::sleep_for(delay);
      errorLed = 0;
      ThisThread::sleep_for(delay);
    }
  }
};

#endif // ERROR_INDICATOR_H
