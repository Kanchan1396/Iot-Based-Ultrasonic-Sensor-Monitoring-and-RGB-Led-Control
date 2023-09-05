#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Galaxy M01 Core1975";
const char* password = "1234567890";

// Define GPIO pins for the RGB LED
const int redPin = D1;
const int greenPin = D2;
const int bluePin = D3;
const int trigPin = 12;
const int echoPin = 14;

// Define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connected to WiFi");

  // Define web server routes
  //server.on("/", HTTP_GET, handleRoot);
  //server.on("/setcolor", HTTP_POST, handleSetColor);

  server.begin();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;

  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  delay(1000);

  //server.on("/hello_distance", HTTP_GET, handleRoot);
  //server.on("/hello", HTTP_GET, handleHello);

  server.on("/hh", HTTP_GET, []() {
  server.sendHeader("Access-Control-Allow-Origin", "*"); // Allow requests from any origin
  server.send(200, "text/plain", String(distanceCm));
});

  server.on("/setcolor", HTTP_POST, handleSetColor);

  server.handleClient();
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>RGB LED Control</h1>";
  html += "<form action='/setcolor' method='POST'>";
  html += "Red: <input type='number' name='red' min='0' max='255'><br>";
  html += "Green: <input type='number' name='green' min='0' max='255'><br>";
  html += "Blue: <input type='number' name='blue' min='0' max='255'><br>";
  html += "<input type='submit' value='Set Color'>";
  html += "</form></body></html>";
  
  server.send(200, "text/html", html);
}

void handleSetColor() {
  int red = server.arg("red").toInt();
  int green = server.arg("green").toInt();
  int blue = server.arg("blue").toInt();
  
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);

  server.send(200, "text/plain", "RGB LED color set");
}
