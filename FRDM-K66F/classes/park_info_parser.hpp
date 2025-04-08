#ifndef PARK_INFO_PARSER_H
#define PARK_INFO_PARSER_H

#include "parking_info.hpp"
#include <ArduinoJson.h>
#include <string>

class ParkInfoParser {
private:
  std::string rawData; // Raw JSON string
  JsonDocument doc;    // Whole JSON
  JsonObject rootObj;  // JSON Object for the root

  bool status;             // Status flag
  ParkingInfo parkingInfo; // Parking information object

public:
  ParkInfoParser() {}
  ParkInfoParser(const std::string &rawData) : rawData(rawData) {}

  void setRawData(const std::string &rawData) { this->rawData = rawData; }

  bool parseRawData() {
    // Deserialize the entire JSON into doc
    DeserializationError error = deserializeJson(doc, rawData);
    if (error) {
      printf("Error deserializing JSON: %s\n", error.c_str());
      return false;
    }

    // Access the root JSON object
    rootObj = doc.as<JsonObject>();

    // Extract values from the root object
    if (rootObj["status"] && rootObj["data"]) {
      this->status = rootObj["status"];

      // Deserialize the nested JSON inside the 'data' field into a JsonObject
      std::string dataStr = rootObj["data"].as<std::string>();
      error = deserializeJson(doc, dataStr);
      if (error) {
        printf("Error deserializing nested data JSON: %s\n", error.c_str());
        return false;
      }

      // Now extract the relevant data
      JsonObject dataObj = doc.as<JsonObject>(); // Nested JSON

      if (this->status) {
        // Extract individual data fields from the 'data' JSON object
        if (dataObj["total_spots"]) {
          this->parkingInfo.totalSpots = dataObj["total_spots"];
        }
        if (dataObj["parked_count"]) {
          this->parkingInfo.parkedSpots = dataObj["parked_count"];
        }
        if (dataObj["empty_count"]) {
          this->parkingInfo.emptySpots = dataObj["empty_count"];
        }
        if (dataObj["empty_indices"]) {
          JsonArray indices = dataObj["empty_indices"].as<JsonArray>();
          this->parkingInfo.empty_indices = indices;
        }
      }
    } else {
      printf("Error: Missing or invalid data\n");
      return false;
    }

    return true;
  }

  void printParsedResult() {
    // Print the status
    printf("Status: %s\n", status ? "true" : "false");

    // Print the parking information
    printf("Total spots: %d\n", parkingInfo.totalSpots);
    printf("Parked spots: %d\n", parkingInfo.parkedSpots);
    printf("Empty spots: %d\n", parkingInfo.emptySpots);

    // Print the empty indices
    printf("Empty indices: ");
    for (JsonVariant v : parkingInfo.empty_indices) {
      printf("%d ", v.as<int>());
    }
    printf("\n");
  }

  const ParkingInfo &getParkingInfo() const { return parkingInfo; }
};

#endif // End of the header guard
