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
/*
String page  = "";
String prepareHtmlPage(){
  String htmlPage = 
    String()
  }
*/
void setup() {
  // put your setup code here, to run once:
  //Basic HTML webpage
  page = "<h1>Simple NodeMCU Web Server</h1><p>"
  + "<a href= \"BrewButton\"><button>Brewing Button</button></a>&nbsp;"
  + "<a href=\"LEDOff\"><button>OFF</button></a></p>";

  //setting the pins as input or output
  pinMode(BrewLightPin, INPUT_PULLDOWN_16);
  pinMode(BrewButPin, OUTPUT);

  //set coffee brewing to low so it's not brewing yet
  digitalWrite(BrewButPin, LOW)

  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //If server is asking for default uri, send page
  server.on("/", [](){
    server.send(200, "text/html", page);
  });

  //When the client requests the brew button uri through pressing the Brew Button on the webpage,
  //We simulate a brew button press on the keurig through digitally writing the pin to high and then
  //low with a short delay.
  server.on("/BrewButton", [](){
    server.send(200, "text/html", page);
    digitalWrite(BrewButPin, HIGH);
    delay(1000);
    digitalWrite(BrewButPin, LOW);
  });
}

void loop() {
  // put your main code here, to run repeatedly:
    server.handleClient ();
}
