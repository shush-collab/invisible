#include <WiFi.h>
#include <HTTPClient.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;


constexpr char SSID[] = "wifidev";
constexpr char PASS[] = "00000001";
constexpr char URL[] = "http://192.168.180.173:5000/text";

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && (millis() - start < 1500)) { delay(10); }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  bool connected = connectToWiFi();
  if (!connected) {
    Serial.println("Giving up on WiFi after retries");
    return;
  }

  HTTPClient http;
  http.setConnectTimeout(3000);
  http.setTimeout(10000);
  Serial.print("GET ");
  Serial.println(URL);
  if (http.begin(URL)) {
    int httpCode = http.GET();
    Serial.printf("HTTP code: %d\n", httpCode);
    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println("Payload:");
      Serial.println(payload);
      keyboardStart();
      typeLikeHuman(payload);
      keyboardEnd();
    } else {
      Serial.printf("GET failed: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("HTTP begin() failed");
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

bool connectToWiFi() {
  const int maxRetries = 3;
  for (int retry = 1; retry <= maxRetries; ++retry) {
    Serial.printf("WiFi attempt %d/%d\n", retry, maxRetries);
    WiFi.begin(SSID, PASS);

    unsigned long t0 = millis();
    const unsigned long timeout = 15000;
    while (WiFi.status() != WL_CONNECTED && (millis() - t0) < timeout) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {

      Serial.print("Connected. IP: ");
      Serial.println(WiFi.localIP());
      return true;
    } else {
      Serial.printf("Failed (status: %d). Retrying...\n", WiFi.status());
      WiFi.disconnect();
      delay(1000);
    }
  }
  return false;
}


inline void keyboardStart() {
  USB.begin();
  Keyboard.begin();
}

inline void keyboardEnd() {
  Keyboard.releaseAll();
  delay(50);
}
inline void typeLikeHuman(const String& text) {
  const size_t n = text.length();
  const uint8_t TAB_SPACES = 4;
  for (size_t i = 0; i < n; i++) {
    char c = text[i];

    if (c == '\t') {
        Keyboard.write(' ');
        delay(random(5, 15));
    }else{
      Keyboard.write(c);
    }

    
    delay(random(100, 300));

    if ((random(10) == 0) && c != ' ' && c != '\n') {
      Keyboard.write('a' + random(26));
      delay(random(90, 250));
      Keyboard.write(KEY_BACKSPACE);
      delay(random(100, 200));
    }
    if (random(60) == 0) {
      delay(random(80, 220));
    }
  }
}

void loop() {}