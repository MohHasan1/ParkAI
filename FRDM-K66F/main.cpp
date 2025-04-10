#include "mbed.h"
// #include "mock_esp32_cam.hpp"
#include "error_indicator.hpp"
#include "park_info_parser.hpp"
#include "parking_display.hpp"
#include "parking_gate.hpp"
#include "parking_indicator.hpp"
#include "uart_handler.hpp"

int main() {
  UartHandler uart(D1, D0); // esp32cam uart
  //   MockESP32CAM uart; // mocks esp32cam uart

  ParkingIndicator indicator({D2, D3, D4, D5, D6, D7});
  ParkingGate gate(D8);
  ParkingDisplay display(D14, D15);
  //   ErrorIndicator error(D11);

  // TODO: start sequence action function
  printf("Debug: Init. modules.\n");

//   error.blinkError(10, 1000ms);
  gate.open();
  ThisThread::sleep_for(1000ms);
  indicator.turnAllOff();
  gate.close();
  ThisThread::sleep_for(1000ms);
  gate.open();
  indicator.turnAllOn();
  ThisThread::sleep_for(1000ms);
  indicator.turnAllOff();
  gate.close();
  ThisThread::sleep_for(1000ms);
  gate.open();
  indicator.turnAllOn();
  ThisThread::sleep_for(1000ms);

  while (true) {
    // error.clearError();

    printf("Debug: Sending data via UART...\n");
    bool isOk = uart.sendData("send");
    if (!isOk) {
      printf("Error: Can't send data via UART.\n");
    //   error.signalError();
      continue;
    }

    bool isSuccess = uart.readDataWithTimeout(6000);
    if (!isSuccess) {
      printf("Error: Can't read UART data or No new Data.\n");
    //   error.signalError();
      continue;
    }

    std::string data = uart.getStoredData();
    printf("Debug: Received data: %s\n", data.c_str());

    ParkInfoParser parser(data);
    bool isparsed = parser.parseRawData();
    if (!isparsed) {
      printf("Error: Can't parse data.\n");
    //   error.signalError();
      continue;
    }

    printf("Debug: Parsed parking info:\n");
    parser.printParsedResult();

    // Update the periphirals:
    ParkingInfo info = parser.getParkingInfo();

    // Update Led
    indicator.update(info);

    // update servo motor
    gate.update(info);

    // update lcd
    display.update(info);
  }
}
