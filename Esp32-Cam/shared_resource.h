#ifndef SHARED_RESOURCE_H
#define SHARED_RESOURCE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

class SharedResource {
private:
  String value;
  SemaphoreHandle_t mutex;

public:
  SharedResource() {
    mutex = xSemaphoreCreateMutex();
  }

  void write(const String& data) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      value = data;
      xSemaphoreGive(mutex);
    }
  }

  String read() {
    String result;
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      result = value;
      xSemaphoreGive(mutex);
    }
    return result;
  }

  SemaphoreHandle_t getMutex() const {
    return mutex;
  }
};

#endif  // SHARED_RESOURCE_H