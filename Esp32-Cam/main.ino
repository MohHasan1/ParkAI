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
  Serial.println("Debug: Connecting to Internet.");
  bool netRes = internet.connect();
  if (!netRes) {
    sendErrorPayload("Internet connection failed");
    return;
  }

  // Initialize ESP32CAM:
  Serial.println("Debug: Init. the camera.");
  bool espRes = esp32cam.init();
  if (!espRes) {
    sendErrorPayload("Camera initialization failed");
    return;
  }

  // Start the camera thread - core1:
  Serial.println("Debug: Starting capturing thread.");
  esp32cam.startContinuousCaptureTask(&shared);

  Serial.println("Debug: Starting Main thread.");
}

void loop() {
  unsigned long start = millis();
  String signal = "";

  // Wait until data is available or timeout occurs
  while (!Serial.available()) {
    if (millis() - start > 5000) {
      return;
    }
    delay(10);
  }

  // Read the signal
  signal = Serial.readStringUntil('\n');
  signal.trim();
  if (signal != "send") return;

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
  JsonDocument resDoc;
  JsonHandler::deserializeJsonStringAndReturn(res, resDoc);
  if (!resDoc["status"]) {
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
