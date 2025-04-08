#ifndef MOCK_ESP32_CAM_HPP
#define MOCK_ESP32_CAM_HPP

#include "mbed.h"
#include <string>

class MockESP32CAM {
private:
  bool isSignaled = false;

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
  MockESP32CAM() {}
  ~MockESP32CAM() { this->stopSignalThread(); }

  bool readData() {
    if (this->isSignaled) {
      this->isSignaled = false;
      return true;
    }
    return false;
  }

  std::string getStoredData() {
    //{"data":"{\"empty_count\":1,\"empty_indices\":[1],\"parked_count\":5,\"parked_indices\":[2,3,4,5,6],\"total_spots\":6}","status":true}
    return R"(
    {
        "data": {
            "total_spots": 4,
            "parked_count": 2,
            "empty_count": 2,
            "empty_indices": [1, 3]
        },
        "status": true
    }
    )";
  }

  bool sendData(const std::string &signal) {
    if (signal == "send") {
      this->isSignaled = true;
      return true;
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

#endif // MOCK_ESP32_CAM_HPP


// Received data: {"data":"{\"empty_count\":2,\"empty_indices\":[3,4],\"parked_count\":2,\"parked_indices\":[1,2],\"total_spots\":4}","status":true}
// Status: true
// Total spots: 0
// Parked spots: 0
// Empty spots: 0
// Empty indices: 
// Total: 0, Parked: 0
