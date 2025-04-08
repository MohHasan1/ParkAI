#ifndef ESP32CAM_H
#define ESP32CAM_H

#include "base64.h"
#include "utils.h"
#include "esp_camera.h"
#include "shared_resource.h"

// Pin definitions for the ESP32-CAM AI-Thinker module
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

class Esp32Cam {
private:
  camera_config_t config;
  camera_fb_t* fb;

  struct CaptureArgs {
    Esp32Cam* cam;
    SharedResource* shared;
    int interval;
  };

  static void continuousCaptureTask(void* param) {
    auto* args = static_cast<CaptureArgs*>(param);
    while (true) {
      camera_fb_t* fb = esp_camera_fb_get();
      if (!fb) {
        sendErrorPayload("Camera capture failed");
      } else {
        args->shared->write(base64::encode(fb->buf, fb->len));
        esp_camera_fb_return(fb);
      }

      vTaskDelay(args->interval / portTICK_PERIOD_MS);
    }

    delete args;
    vTaskDelete(nullptr);
  }

public:

  Esp32Cam() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;  // Set frame size, can be changed (FRAMESIZE_QVGA, FRAMESIZE_VGA, etc.)
    config.jpeg_quality = 10;           // JPEG Quality, 0-63 lower is better
    config.fb_count = 1;                // Number of frame buffers (1 or 2)
  }

  // Method to initialize the camera
  bool init() {
    if (esp_camera_init(&config) != ESP_OK) {
      return false;
    }
    return true;
  }

  void startContinuousCaptureTask(SharedResource* sharedResource, int intervalMs = 1000) {
    CaptureArgs* args = new CaptureArgs{ this, sharedResource, intervalMs };
    xTaskCreatePinnedToCore(
      continuousCaptureTask,  // function to run in the task
      "CaptureTask",          // task name
      4096,                   // stack size in words (4096 = ~16KB)
      args,                   // arguments to the task
      1,                      // priority (1 = low, but not idle)
      nullptr,                // task handle (not storing it here)
      1                       // core to pin it to (Core 1)
    );
  }

  // Capture an image and store it
  bool capture(int delayMs = 0) {
    if (delayMs > 0) {
      delay(delayMs);  // Wait before capturing if delay is provided
    }

    if (fb) {
      esp_camera_fb_return(fb);
      fb = nullptr;
    }

    fb = esp_camera_fb_get();
    if (!fb) {
      return false;
    }

    return true;
  }

  // Return the captured image as a Base64 string
  String getBase64Image() {
    if (!fb) {
      return "";
    }
    // base64::encode((const uint8_t*)fb->buf, fb->len)
    return base64::encode(fb->buf, fb->len);
  }

  // Method to release the frame buffer after capture
  void release(camera_fb_t* fb) {
    if (fb) {
      esp_camera_fb_return(fb);
    }
  }

  void reset() {
    esp_camera_deinit();
    esp_camera_init(&config);
  }
};

#endif