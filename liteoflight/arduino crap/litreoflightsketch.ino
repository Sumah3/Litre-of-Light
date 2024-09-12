#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LDR_PIN A0 // ESP8266 pin connected to LDR (analog pin)
#define LED1_PIN D1 // ESP8266 pin connected to LEDs (GPIO5)
#define THRESHOLD1 900 // turn on LEDs under this light level

const char* ssid = "amongus";
const char* password = "bruh22";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/ldr", handleLDR);
  server.on("/toggleLED", handleToggleLED);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html><head><title>ESP8266 Web Server</title></head><body><h1>ESP8266 Web Server</h1><p>LDR Value: <span id='ldrValue'>0</span></p><button onclick='toggleLED()'>Toggle LED</button><script>function fetchLDRValue() { fetch('/ldr').then(response => response.text()).then(data => { document.getElementById('ldrValue').innerText = data; }); } function toggleLED() { fetch('/toggleLED').then(response => response.text()).then(data => { alert(data); }); } setInterval(fetchLDRValue, 1000);</script></body></html>");
}

void handleLDR() {
  int ldrValue = analogRead(LDR_PIN);
  server.send(200, "text/plain", String(ldrValue));
}

void handleToggleLED() {
  static bool ledState = false;
  ledState = !ledState;
  digitalWrite(LED1_PIN, ledState ? HIGH : LOW);
  server.send(200, "text/plain", ledState ? "LED is ON" : "LED is OFF");
}
