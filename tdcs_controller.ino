#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "tDCS-Device";
const char* password = "12345678";

ESP8266WebServer server(80);

// Define pins for stimulation control
const int stimPin = D1;      // Replace with your actual pin
const int polarityPin = D2;  // Optional for polarity control (anodal/cathodal)

void handleStart() {
  String mode = server.arg("mode");
  float current = server.arg("current").toFloat();
  int duration = server.arg("duration").toInt(); // in minutes
  String polarity = server.arg("polarity");

  // Debug output
  Serial.println("Mode: " + mode);
  Serial.println("Current: " + String(current) + " mA");
  Serial.println("Duration: " + String(duration) + " minutes");
  Serial.println("Polarity: " + polarity);

  // Control stimulation pins (simulate control)
  digitalWrite(stimPin, HIGH);
  if (polarity == "anodal") {
    digitalWrite(polarityPin, HIGH);
  } else {
    digitalWrite(polarityPin, LOW);
  }

  // Set a timer to stop after duration (not blocking)
  unsigned long stimEndTime = millis() + (duration * 60UL * 1000UL);

  server.send(200, "text/plain", "tDCS শুরু হয়েছে। মোড: " + mode + ", সময়: " + String(duration) + " মিনিট");

  // Background task in loop() will handle stopping
}

unsigned long stimEnd = 0;

void setup() {
  Serial.begin(115200);
  pinMode(stimPin, OUTPUT);
  pinMode(polarityPin, OUTPUT);
  digitalWrite(stimPin, LOW);
  digitalWrite(polarityPin, LOW);

  // Setup Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started: " + String(ssid));
  Serial.println("IP address: " + WiFi.softAPIP().toString());

  // Define route
  server.on("/start", handleStart);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Stop stimulation after time (optional improvement)
  static unsigned long stimStopTime = 0;
  if (digitalRead(stimPin) == HIGH && stimStopTime == 0) {
    stimStopTime = millis() + (5UL * 60UL * 1000UL); // default 5 mins safety
  }

  if (stimStopTime != 0 && millis() > stimStopTime) {
    digitalWrite(stimPin, LOW);
    digitalWrite(polarityPin, LOW);
    stimStopTime = 0;
    Serial.println("Stimulation ended.");
  }
}