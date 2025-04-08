#ifndef UTILS_H
#define UTILS_H

#include "json_handler.h"

inline String createJsonPayload(const String& data, bool status) {
  JsonDocument doc;
  doc["data"] = data;
  doc["status"] = status;

  String output;
  serializeJson(doc, output);
  return output;
}

inline void sendErrorPayload(const String& errorMessage) {
  String payload = createJsonPayload(errorMessage, false);
  Serial.println(payload);
}

inline void sendSuccessPayload(const String& data) {
  String payload = createJsonPayload(data, true);
  Serial.println(payload);
}


#endif  // UTILS_H
