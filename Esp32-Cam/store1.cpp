// #include "net.h"
// #include "open_ai.h"
// #include "esp_32_cam.h"
// #include "shared.h"

// Net internet;
// OpenAI openai;
// SharedResource shared;
// Esp32Cam esp32cam;

// bool espRes;

// void setup() {
//   Serial.begin(115200);

//   // Connect to the internet
//   internet.connect();

//   // Initialize ESP32CAM
//   espRes = esp32cam.init();
//   if (!espRes) {
//     Serial.println("Camera initialization failed!");
//     return;
//   }

//   // start the camera thread
//   esp32cam.startContinuousCaptureTask(&shared);
// }

// void loop() {
//   // delay(3000);
//   // Serial.println(shared.read());
  
//   delay(5000);
//   bool mock = true;

//   // if if mock signal send via uart.
//   if (mock) {
//     Serial.println("Sending base64 to parkAi model");
//     // read from shared resoucse
//     String base64 = shared.read();
//     // hhtp req to localhost
//     String httpPayload = "{\"image_base64\": \"" + base64 + "\"}";
//     internet.setHeaders({{"Content-Type", "application/json"}});
//     String res = internet.POST("http://172.20.10.2:5050/process", httpPayload); 

//     // extract the data from http-response and print
//     Serial.println("Response: " + res);

//     // send the string via uart - later
//   }

//   // if (!internet.isConnected() || !espRes) {
//   //   Serial.println("Setup failed!");
//   //   return;
//   // }

//   // Capture an image with a 5-second delay
//   // bool capRes = esp32cam.capture(5000);
//   // if (!capRes) {
//   //   Serial.println("Camera capture failed!");
//   //   return;
//   // }

//   // Convert to Base64
//   // String base64Image = esp32cam.getBase64Image();
//   // Serial.println(base64Image);

//   // Set prompt for OpenAI
//   // openai.setPrompt("How many cars do you see? If you see no cars or something else, say 'no cars'. Respond in JSON.");

//   // Prepare HTTP request payload and headers
//   // String httpPayloadForImage = openai.createHttpPayloadForImage(base64Image);
//   // std::map<String, String> httpHeaderForImage = openai.createHttpHeaderForImage();

//   // Send request to OpenAI
//   // internet.setHeaders(httpHeaderForImage);
//   // String res = internet.POST(openai.getOpenai_endpoint(), httpPayloadForImage);

//   // Parse and print the result
//   // String result = openai.parseResult(res);
// }

////////////////////json
// #include "net.h"
// #include "open_ai.h"
// #include "esp_32_cam.h"
// #include "shared_resource.h"
// #include "json_handler.h"
// #include <ArduinoJson.h>

// Net internet;
// OpenAI openai;
// SharedResource shared;
// Esp32Cam esp32cam;
// JsonHandler json;

// void setup() {
//   Serial.begin(9600);   // Serial monitor for debugging
//   Serial1.begin(115200);  // Serial monitor for UART

//   // Connect to the internet:
//   bool netRes = internet.connect();
//   if (!netRes) {
//     Serial.println("Internet connection failed!");
//     Serial1.println("error");
//     return;
//   }

//   // Initialize ESP32CAM:
//   bool espRes = esp32cam.init();
//   if (!espRes) {
//     Serial.println("Camera initialization failed!");
//     Serial1.println("error");
//     return;
//   }

//   // Start the camera thread:
//   esp32cam.startContinuousCaptureTask(&shared);
// }

// void loop() {
//   delay(5000);
//   bool mock = true;

//   if (mock) {
//     Serial.println("Sending base64 to parkAi model...");

//     // 1. Read base64 image from the shared resource:
//     String base64 = shared.read();

//     // 2. HTTP POST request to localhost (ParkAI Model):
//     String httpPayload = "{\"image_base64\": \"" + base64 + "\"}";
//     internet.setHeaders({ { "Content-Type", "application/json" } });

//     String res = internet.POST("http://172.20.10.2:5050/process", httpPayload);
//     if (res == "error") Serial1.println("error");

//     // 3. Process the json response:
//     JsonDocument doc;
//     res.trim();

//     DeserializationError error = deserializeJson(doc, res);
//     if (error) {
//       Serial.println("Failed to deserialize JSON: " + String(error.c_str()));
//       Serial1.println("error");
//     }

//     String jsonString;
//     serializeJson(doc, jsonString);

//     // 4. Send the serialized json to frdm-k664 via UART:
//     Serial1.println(jsonString);
//   }
// }