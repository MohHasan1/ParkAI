#ifndef NET_H
#define NET_H

#include <map>
#include <WiFi.h>
#include <HTTPClient.h>


class Net {
private:
  HTTPClient http;

  const char *ssid;
  const char *password;

  std::map<String, String> storedHeaders;

  void applyHeaders() {
    for (const auto &header : storedHeaders) {
      http.addHeader(header.first, header.second);
    }
  }

public:
  Net() {
    this->ssid = "hasan";
    this->password = "xilontyreos55";
  }

  // Constructor with custom credentials
  Net(const char *ssid, const char *pass) {
    this->ssid = ssid;
    this->password = pass;
  }

  ~Net() {
    http.end();
  }


  // Connect to WiFi
  bool connect() {
    // Serial.print("Connecting to WiFi");
    WiFi.begin(this->ssid, this->password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      // Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      // Serial.println("\nConnected!");
      // Serial.print("IP Address: ");
      // Serial.println(WiFi.localIP());
      return true;
    } else {
      return false;
    }
  }

  // Set http headers
  void setHeaders(const std::map<String, String> &headers) {
    this->storedHeaders = headers;
  }

  // Function to send a POST request
  String POST(const String &url, const String &payload) {
    if (!this->isConnected()) {
      Serial.println("\nError: WiFi not connected. Cannot send POST request.");
      return String("error");
    }

    http.begin(url);
    this->applyHeaders();
    int httpResponseCode = http.POST(payload);

    String response = "";
    if (httpResponseCode > 0) {
      response = http.getString();
      // Serial.print("HTTP Response code: ");
      // Serial.println(httpResponseCode);
    } else {
      // Serial.print("Error: Error on sending POST: ");
      // Serial.println(httpResponseCode);
      return String("error");
    }

    http.end();
    return response;
  }

  // Check the connection status
  bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
  }
};

#endif  // End of header guard