#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "Myhouse";
const char* password = "redlotus1313";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

int BrewLightPin = D0;
int BrewButPin = D2;
//Using GPIO5 (D1) for Brewing Light Input
//Using GPIO6 (D2) for Brewing Button Output

String page  = "";

void setup() {
  // put your setup code here, to run once:
  //Basic HTML webpage
  page = "<h1>Simple NodeMCU Web Server</h1><p><a href= \"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";

  //setting the pins as input or output
  pinMode(BrewLightPin, INPUT_PULLDOWN_16);
  pinMode(BrewButPin, OUTPUT);

  //set coffee brewing to low so it's not brewing yet
  digitalWrite(BrewButPin, LOW)
  
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
