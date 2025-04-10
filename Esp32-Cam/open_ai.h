#ifndef OPEN_AI_H
#define OPEN_AI_H

#include <map>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class OpenAI {
private:
  const char* openai_api_key;
  const char* openai_endpoint;

  String prompt;

public:
  OpenAI() {
    this->openai_api_key = "";
    this->openai_endpoint = "https://api.openai.com/v1/chat/completions";
    this->prompt = "";
  }

  // Constructor that allows setting a custom OpenAI API key
  OpenAI(const char* openai_api_key) {
    this->openai_api_key = openai_api_key;
    this->openai_endpoint = "https://api.openai.com/v1/chat/completions";
    this->prompt = "";
  }

  const char* getOpenai_endpoint() {
    return this->openai_endpoint;
  }

  // Setter for prompt (question)
  void setPrompt(const String& newPrompt) {
    this->prompt = newPrompt;
  }

  // Create payload using image and the prompt set using the setter
  String createHttpPayloadForImage(String base64Image) {
    Serial.println("Preparing Http-Payload with Image...");

    // Check if prompt has been set
    if (this->prompt == "") {
      Serial.println("Error: Prompt is empty!");
      return "";
    }

    // Prepare image URL in base64 format
    String img_url = "data:image/jpeg;base64," + base64Image;
    Serial.println(img_url);

    // Create JSON payload
    DynamicJsonDocument doc(4096);
    doc["model"] = "gpt-4o";
    JsonArray messages = doc.createNestedArray("messages");
    JsonObject message = messages.createNestedObject();
    message["role"] = "user";
    JsonArray content = message.createNestedArray("content");

    // Add text content (prompt)
    JsonObject textContent = content.createNestedObject();
    textContent["type"] = "text";
    textContent["text"] = this->prompt;

    // Add image content
    JsonObject imageContent = content.createNestedObject();
    imageContent["type"] = "image_url";
    JsonObject imageUrlObject = imageContent.createNestedObject("image_url");
    imageUrlObject["url"] = img_url;
    imageUrlObject["detail"] = "auto";

    // Add additional parameters
    doc["max_tokens"] = 400;

    // Serialize and return the JSON payload
    String jsonPayload;
    serializeJson(doc, jsonPayload);
    return jsonPayload;
  }

  std::map<String, String> createHttpHeaderForImage() {
    return {
      { "Content-Type", "application/json" },
      { "Authorization", String("Bearer ") + this->openai_api_key }
    };
  }

  String parseResult(const String& res) {
    DynamicJsonDocument responseDoc(4096);
    DeserializationError error = deserializeJson(responseDoc, res);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      return "";
    }

    // Extract the content safely
    if (responseDoc.containsKey("choices") && responseDoc["choices"][0]["message"].containsKey("content")) {
      String responseContent = responseDoc["choices"][0]["message"]["content"].as<String>();
      Serial.println("[ChatGPT] Parsed response: " + responseContent);
      return responseContent;
    }

    Serial.println("[ChatGPT] No valid content found in response.");
    return "";
  }
};
#endif