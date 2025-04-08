#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h>

class JsonHandler {
private:
  JsonDocument doc;       // Internal JsonDocument
  String storedJson;      // Raw input JsonString
  String serializedJson;  // JSON string from serialization
  bool isError = false;   // Error flag

public:
  JsonHandler(const String& jsonString = "")
    : storedJson(jsonString) {
    if (storedJson.length() > 0) {
      deserializeStoredJson();
    }
  }

  // Serialize the internal JsonDocument to serializedJson string
  void serializeStoredJsonDoc() {
    serializedJson = "";
    serializeJson(doc, serializedJson);
  }

  // Deserialize the storedJson string into the internal JsonDocument (parsing)
  void deserializeStoredJson() {
    // 1. Trim:
    storedJson.trim();
    // 2. Convert stored-json string to json-doc and store it:
    DeserializationError error = deserializeJson(doc, storedJson);
    // 3. Handle error:
    if (error) {
      // Serial.println("Error: In JsonHandler constructor.");
      isError = true;
    }
  }

  // Set and deserialize a new JSON string into the internal JsonDocument (parsing)
  void deserializeJsonStringAndStore(const String& inputJson) {
    // 1. Update the StoredJson:
    storedJson = inputJson;
    // 2. Trim:
    storedJson.trim();
    // 3. Deserialize the json-string and store it in teh json-doc:
    DeserializationError error = deserializeJson(doc, storedJson);
    if (error) {
      // Serial.println("Error: In deserializeJsonStringAndStore.");
      isError = true;
    }
  }

  // Static: Convert JsonDocument to JSON string (returns json string)
  static String serializeJsonAndReturn(const JsonDocument& json) {
    String output;
    serializeJson(json, output);
    return output;
  }


  // Static: Converts JSON String to JsonDocument and store into a provided JsonDocument reference
  static DeserializationError deserializeJsonStringAndReturn(const String& jsonString, JsonDocument& externalDoc) {
    return deserializeJson(externalDoc, jsonString);
  }

  // Accessor for internal JsonDocument
  JsonDocument& getJsonDoc() {
    return this->doc;
  }

  // Accessor for serialized JSON string
  String& getSerializedJson() {
    return this->serializedJson;
  }

  String& getStoredJsonString() {
    return this->storedJson;
  }

  bool hasError() const {
    return this->isError;
  }

  void clearError() {
    this->isError = false;
  }
};

#endif  // JSON_HANDLER_H
