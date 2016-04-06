#include <ESP8266WiFi.h>
#include <Q2HX711.h>

const char* ssid = "kuanfamily";
const char* password = "homesweethome";
const char* host = "84135909.ngrok.io";
const int httpPort = 80;


const byte hx711_data_pin = 4;
const byte hx711_clock_pin = 5;
float tareFactor = 0;
float calibFactor = -240.310077519;
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

int tareButtonPin = 2;
bool currentState = true;


void tare() {
  tareFactor = hx711.read();
}

void buttonTare(){
 bool buttonPressed = digitalRead(tareButtonPin);

  if (buttonPressed != currentState) {
    if (buttonPressed) {
      Serial.println("Taring");
      tare();
      delay(200);
    }
    currentState = buttonPressed;
  } 
}

float readLikeABoss() {
  float cool = (hx711.read()-tareFactor)/calibFactor;
  
  return(cool/1000);
}

void setup() {
  Serial.begin(9600);
  delay(50);
  tare();
  pinMode(tareButtonPin, INPUT);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  Serial.print(readLikeABoss());
  Serial.println(" | TareFactor: " + String(tareFactor));
  
  //  Taring stuff
  if(Serial.available()){
    char temp = Serial.read();
    if(temp == 't'){
      tare();
    }
  }
  buttonTare();

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String data = "weight="+String(readLikeABoss());
  data += "&id=james";
  String url = "POST / HTTP/1.1\r\nHost: ";
  url += host;
  url += "\r\nConnection: close";
  url += "\r\nContent-Length: " + String(data.length());
  url += "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n";
  url += data;
  Serial.println(url);
  client.print(url);
  delay(10);
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
//  Serial.println("closing connection");
  delay(5000);
}


