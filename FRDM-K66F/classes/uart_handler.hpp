#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include "mbed.h"
#include <chrono>
#include <string>

class UartHandler {
private:
  BufferedSerial serial_port;
  std::string data;

  //   Thread sendThread;
  //   bool sendThreadRunning = false;
  //   bool sendThreadShouldStop = false;

  //   void sendSignalLoop() {
  //     while (!sendThreadShouldStop) {
  //       bool success = this->sendData("send");
  //       if (!success) {
  //         printf("UartHandler Error: Failed to send signal.\n");
  //       }
  //       ThisThread::sleep_for(2000ms);
  //     }
  //     sendThreadRunning = false;
  //   }

public:
  UartHandler(PinName tx, PinName rx, int baud_rate = 115200)
      : serial_port(tx, rx, baud_rate) {}
  //   ~UartHandler() { this->stopSignalThread(); }

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
      if (bytes_written == static_cast<ssize_t>(payload.length())) {
        printf("Sent data: %s\n", payload.c_str());
        return true;
      } else {
        printf("Failed to send data: %s\n", payload.c_str());
      }
    }
    return false;
  }

  //   void startSignalThread() {
  //     if (sendThreadRunning) {
  //       printf("Send Thread is already running.\n");
  //       return;
  //     }

  //     sendThreadShouldStop = false;
  //     sendThreadRunning = true;

  //     sendThread.start([this]() { this->sendSignalLoop(); });
  //   }

  //   void stopSignalThread() {
  //     if (sendThreadRunning) {
  //       sendThreadShouldStop = true; // Signal the thread to stop
  //       sendThread.join();           // Wait for the thread to finish
  //       sendThreadRunning = false;   // Mark the thread as stopped
  //     }
  //   }

  // Function to read data with a timeout
  bool readDataWithTimeout(int timeout_ms) {
    char c;
    this->data.clear();
    Timer timer;
    timer.start();

    while (timer.elapsed_time() < std::chrono::milliseconds(timeout_ms)) {
      if (serial_port.readable()) {
        if (serial_port.read(&c, 1) > 0) {
          if (c == '\n') {
            timer.stop();
            return true; // Line end received
          } else {
            data.push_back(c);
          }
        }
      }
      ThisThread::sleep_for(10ms); // Allow other threads some CPU time
    }

    timer.stop();
    return false; // Timeout reached without newline
  }
};

#endif // UART_HANDLER_H
