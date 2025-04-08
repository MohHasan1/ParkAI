#include "mbed.h"
#include "mock_esp32_cam.hpp"
#include "park_info_parser.hpp"
#include "parking_gate.hpp"
#include "parking_indicator.hpp"
#include "uart_handler.hpp"

int main() {
  UartHandler uart(D1, D0); // esp32cam uart
  //   MockESP32CAM uart; // mocks esp32cam uart

  ParkingIndicator indicator({D2, D3, D4, D5});
  ParkingGate gate(D6);

  // TODO: start sequence action function

  //   uart.startSignalThread();

  ThisThread::sleep_for(1000ms);
  while (true) {
    printf("Sending data to UART...\n");
    bool isOk = uart.sendData("send");
    if (!isOk) {
      printf("Error: Can't send data via UART.\n");
      continue;
    }

    ThisThread::sleep_for(1000ms);

    bool isSuccess = uart.readData();
    if (!isSuccess) {
      printf("Error: Can't read UART data or No new Data.\n");
      continue;
    }

    std::string data = uart.getStoredData();
    printf("Received data: %s\n", data.c_str());

    ParkInfoParser parser(data);
    bool isparsed = parser.parseRawData();
    if (!isparsed) {
      printf("Error: Can't parse data.\n");
      continue;
    }

    printf("Parsed parking info:\n");
    parser.printParsedResult();

    // Update the periphirals:
    ParkingInfo info = parser.getParkingInfo();

    // Update Led
    indicator.update(info);

    // update servo motor
    gate.update(info);

    // update lcd
    // screen.update(info);
  }
}
