
/*  ChatGPT x Arduino Nano ESP32  */

// This code is designed for an ESP32-based device to perform the following tasks:
// 1. Connect to a WiFi network using the provided credentials (SSID and password).
// 2. Communicate with the OpenAI GPT-3 service to generate responses to prompts.
// 3. Send generated responses to a second ESP32 server at a specified IP address and port.
// 4. Control built-in and external LEDs for visual feedback.
//
// Instructions for usage:
// - Replace "Your_WiFi_SSID" and "Your_WiFi_Password" with your actual WiFi credentials.
// - Replace "Your_OpenAI_API_Key" with a valid OpenAI GPT-3 API key.
// - Ensure that the serverIP and serverPort variables are set correctly for your second ESP32 server.
// - Upload the code to your ESP32 device and open the Arduino IDE Serial Monitor for interaction.
// - Type "GPT" to enter a prompt and receive a GPT-3-generated response.
// - Type "SEND" to send the GPT-3 response to the second ESP32 server.
// - Type "MYCODE" to enter your own code and send it to the server.
// - LEDs will also blink as specified in the code for visual feedback.
//
// Note: This code is a versatile platform combining WiFi connectivity, GPT-3 integration,
// and basic LED control for various applications.

// Load Wi-Fi library
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <String.h>
// Replace with your network credentials
const char* ssid     = "xxx";
const char* password = "xxx";
//chatgpt api
const char* apiKey = "xxx";

const char* serverIP = "192.168.1.83"; // IP address of the second ESP32
const uint16_t serverPort = 80; // Port number on the second ESP32

WiFiClient client;

String response = "";
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  Serial.println("Type 'SEND' to send the response to ESP32-2 or 'GPT' to enter another prompt.");
  Serial.println("OR type 'MYCODE' to send the your own code to the ESP32-2.");
  Gled();
  Rled();
  Bled();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "GPT") {
      Serial.println("Enter a prompt:");
      while (Serial.available() <= 0) {
        delay(100);
      }
      String prompt = Serial.readStringUntil('\n');
      prompt.trim();
      Serial.println(prompt);
      response = sendChatGPTRequest(prompt);
      Serial.println("ChatGPT Response:");
      Serial.println(response);

    Serial.println("Type 'SEND' to send the response to ESP32-2 or 'GPT' to enter another prompt.");
    Serial.println("OR type 'MYCODE' to send the your own code to the ESP32-2.");
    } 
    
    else if (input == "SEND") {
    if (!client.connected()) {
    if (client.connect(serverIP, serverPort)) {
      String sendCode = response;
      client.println(sendCode);
      Serial.println("Code sent to server");
      delay(1000); // Delay to allow server response (optional)
      while (client.available()) {
        String serverResponse = client.readStringUntil('\n');
        Serial.println("Server response: " + serverResponse);
      }
      client.stop();
      Serial.println("Connection closed");
    }
    else {
      Serial.println("Connection failed");
    }
  }
      blinkLed();

  Serial.println("Type 'SEND' to send the response to ESP32-2 or 'GPT' to enter another prompt.");
  Serial.println("OR type 'MYCODE' to send the your own code to the ESP32-2.");
    }
else if (input == "MYCODE")
  {
    Serial.println("Enter your code, and press type 'SEND' for deploying the code on to the ESP32-2");
    while (Serial.available() <= 0) {
        delay(100);
      }
      String myCode = Serial.readStringUntil('\n');
      myCode.trim();
      Serial.println(myCode);
          if (!client.connected()) {
    if (client.connect(serverIP, serverPort)) {
      String sendCode = myCode;
      client.println(sendCode);
      Serial.println("Code sent to server");
      delay(1000); // Delay to allow server response (optional)
      while (client.available()) {
        String serverResponse = client.readStringUntil('\n');
        Serial.println("Server response: " + serverResponse);
      }
      client.stop();
      Serial.println("Connection closed");
    }
    else {
      Serial.println("Connection failed");
    }
  }
      blinkLed();
      
  Serial.println("Type 'SEND' to send the response to ESP32-2 or 'GPT' to enter another prompt.");
  Serial.println("OR type 'MYCODE' to send the your own code to the ESP32-2.");
  }
  }
}

String sendChatGPTRequest(String prompt) {
  String recieved_response= "";
  String apiUrl = "https://api.openai.com/v1/completions";
  String payload = "{\"prompt\":\"" + prompt + "\",\"max_tokens\":100, \"model\": \"text-davinci-003\"}";
  HTTPClient http;
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(apiKey));
  
  int httpResponseCode = http.POST(payload);
  if (httpResponseCode == 200) {
    String response = http.getString();
  
    // Parse JSON response
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["choices"][0]["text"];
    recieved_response = outputText;
    //Serial.println(outputText);
  } else {
    Serial.printf("Error %i \n", httpResponseCode);
  }
  return recieved_response;
}

void blinkLed() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
}

void Gled(){
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  delay(2000);
  digitalWrite(15, LOW);
  delay(2000);
  digitalWrite(15, HIGH);
  delay(2000);
  digitalWrite(15, LOW);
}


void Rled(){
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  delay(2000);
  digitalWrite(14, LOW);
}

void Bled(){
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  delay(2000);
  digitalWrite(16, LOW);
}