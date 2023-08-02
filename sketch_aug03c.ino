#include <SoftwareSerial.h>

const int trigPin = 2;
const int echoPin = 3;
const int wifiTxPin = 4;
const int wifiRxPin = 5;

const int parkingThreshold = 30; // Adjust this value based on your parking space size

SoftwareSerial wifiSerial(wifiTxPin, wifiRxPin);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  wifiSerial.begin(9600);

  // Connect to Wi-Fi network
  connectWiFi("YOUR_SSID", "YOUR_PASSWORD");
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;

  if (distance < parkingThreshold) {
    sendDataToServer("occupied");
  } else {
    sendDataToServer("available");
  }

  delay(5000); // Wait for 5 seconds before sending the next update
}

void connectWiFi(const char* ssid, const char* password) {
  wifiSerial.println("AT+RST");
  delay(1000);

  wifiSerial.println("AT+CWMODE=1");
  delay(1000);

  wifiSerial.print("AT+CWJAP=\"");
  wifiSerial.print(ssid);
  wifiSerial.print("\",\"");
  wifiSerial.print(password);
  wifiSerial.println("\"");
  delay(5000);
}

void sendDataToServer(const char* status) {
  wifiSerial.println("AT+CIPMUX=0");
  delay(1000);

  wifiSerial.print("AT+CIPSTART=\"TCP\",\"YOUR_SERVER_IP\",YOUR_SERVER_PORT");
  delay(2000);

  wifiSerial.println("AT+CIPSEND");
  delay(1000);

  wifiSerial.print("GET /update?status=");
  wifiSerial.print(status);
  wifiSerial.println(" HTTP/1.1");
  wifiSerial.print("Host: YOUR_SERVER_IP");
  wifiSerial.println();
  wifiSerial.println();
  delay(1500);

  wifiSerial.println("AT+CIPCLOSE");
}
