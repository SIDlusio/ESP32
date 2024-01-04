#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Wire.h>
#include <ErriezBMX280.h>

// WiFi ESP32 Connection
const char* ssid = "YourkWIFIssid";
const char* password = "YourSuperPASS";

#define SEA_LEVEL_PRESSURE_HPA 998.25 // Define your City/Region

ErriezBMX280 bmx280 = ErriezBMX280(0x76);
AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!bmx280.begin()) {
        Serial.println("Error Initialize Sensor!");
        while (1);
    }

    // BMX 280 Setup Location Sensor. Read more in docs
    bmx280.setSampling(BMX280_MODE_NORMAL, BMX280_SAMPLING_X16, BMX280_SAMPLING_X2, BMX280_SAMPLING_X1, BMX280_FILTER_X16, BMX280_STANDBY_MS_500);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi Connected.");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
        html += "<title>ESP32 Web Server</title></head><body>";
        html += "<h1>Dane z czujnika BMX280</h1>";
        html += "<p>Temperatura: " + String(bmx280.readTemperature()) + " &deg;C</p>";
        html += "<p>Wilgotność: " + String(bmx280.readHumidity()) + " %</p>";
        html += "<p>Ciśnienie: " + String(bmx280.readPressure() / 100.0) + " hPa</p>";
        html += "<p>Wysokość: " + String(bmx280.readAltitude(SEA_LEVEL_PRESSURE_HPA)) + " m</p>";
        html += "</body></html>";
        request->send(200, "text/html", html);
    });

    server.begin();
}

void loop() {
    // Null, but ASyncSrv
}
