#include "net.h"
#include "utils.h"
#include "esp_32_cam.h"
#include "shared_resource.h"
#include "json_handler.h"
#include <ArduinoJson.h>

Net internet;
SharedResource shared;
Esp32Cam esp32cam;

void setup() {
  Serial.begin(115200);

  // Connect to the internet:
  bool netRes = internet.connect();
  if (!netRes) {
    sendErrorPayload("Internet connection failed");
    return;
  }

  // Initialize ESP32CAM:
  bool espRes = esp32cam.init();
  if (!espRes) {
    sendErrorPayload("Camera initialization failed");
    return;
  }

  // Start the camera thread - core1:
  esp32cam.startContinuousCaptureTask(&shared);
}

void loop() {
  // if (!Serial.available()) return;

  // String command = Serial.readStringUntil('\n');
  // command.trim();

  // if (command != "send") return;

  // delay(2000);  // mock
  delay(1000);

  // 1. Read base64 image from the shared resource:
  String base64 = shared.read();
  if (base64.length() == 0) {
    sendErrorPayload("Empty base64 string");
    return;
  }

  // 2. HTTP POST request to localhost (ParkAI Model):
  String httpPayload = "{\"image_base64\": \"" + base64 + "\"}";
  internet.setHeaders({ { "Content-Type", "application/json" } });

  String res = internet.POST("http://172.20.10.2:5050/process", httpPayload);
  if (res == "error") {
    sendErrorPayload("Failed to get a valid response from ParkAI model");
    return;
  }

  // 3. Process the json response:
  JsonHandler json(res);
  if (json.hasError()) {
    sendErrorPayload("Malformed JSON response from model");
    return;
  }

  // Access the deserialized json:
  JsonDocument doc = json.getJsonDoc();
  if (!doc["status"]) {
    sendErrorPayload(doc["data"]);
    return;
  }

  String data = JsonHandler::serializeJsonAndReturn(doc["data"]);

  // 4. Send the serialized json to frdm-k664 via UART:
  sendSuccessPayload(data);
}
