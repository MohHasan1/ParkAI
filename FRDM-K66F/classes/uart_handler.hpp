#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include "mbed.h"
#include <string>

class UartHandler {
private:
  BufferedSerial serial_port;
  std::string data;

  Thread sendThread;
  bool sendThreadRunning = false;
  bool sendThreadShouldStop = false;

  void sendSignalLoop() {
    while (!sendThreadShouldStop) {
      bool success = this->sendData("send");
      if (!success) {
        printf("UartHandler Error: Failed to send signal.\n");
      }
      ThisThread::sleep_for(2000ms);
    }
    sendThreadRunning = false;
  }

public:
  UartHandler(PinName tx, PinName rx, int baud_rate = 115200) : serial_port(tx, rx, baud_rate) {}
  ~UartHandler() { this->stopSignalThread(); }

  const std::string &getStoredData() const { return data; }
  bool readData() {
    char c;
    this->data.clear();
    while (serial_port.readable()) {
      if (serial_port.read(&c, 1) > 0) {
        if (c == '\n') {
          return true;
        } else {
          data.push_back(c);
        }
      }
    }
    return false;
  }

  bool sendData(const std::string &payload) {
    if (serial_port.writable()) {
      ssize_t bytes_written =
          serial_port.write(payload.c_str(), payload.length());
      return bytes_written == static_cast<ssize_t>(payload.length());
    }
    return false;
  }

  void startSignalThread() {
    if (sendThreadRunning) {
      printf("Send Thread is already running.\n");
      return;
    }

    sendThreadShouldStop = false;
    sendThreadRunning = true;

    sendThread.start([this]() { this->sendSignalLoop(); });
  }

  void stopSignalThread() {
    if (sendThreadRunning) {
      sendThreadShouldStop = true; // Signal the thread to stop
      sendThread.join();           // Wait for the thread to finish
      sendThreadRunning = false;   // Mark the thread as stopped
    }
  }
};

#endif // UART_HANDLER_H
