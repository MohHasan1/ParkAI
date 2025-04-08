#ifndef PARKING_INFO_H
#define PARKING_INFO_H

#include <ArduinoJson.h>

struct ParkingInfo {
  int totalSpots;           // Total number of parking spots
  int parkedSpots;          // Number of occupied (parked) spots
  int emptySpots;           // Number of available spots
  JsonArray empty_indices; // Indices of parked spots (stored in a JSON array)
};

#endif // PARKING_INFO_H
