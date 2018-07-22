#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "Myhouse";
const char* password = "redlotus1313";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

//IMPLEMENTED
int BrewLightPin = D0;
int BrewButPin = D2;
//Using GPIO16 (D0) for Brewing Light Input
//Using GPIO6 (D2) for Brewing Button Output

//UNIMPLEMENTED
int BrewLeftPin = D1;
//GPIO GPIO5 (D1) for Brewing Size Left Directional Button

int BrewRightPin =  D10;
//GPIO1 (D10) for Brewing Size Right Directional Button
// this pin has TXD0

int OnOffPin = D9;
//GPIO3 (D9)for On/Off switch for keurig
// this pin has RXD0

String page  = "";
String prepareHtmlPage(){
  String htmlPage = 
    String("HTTP/1.1 200 OK\r\n") +
    "Content-Type: text/html\r\n" +
    "Connection: close\r\n" +//connection closed after response
    "Refresh: 5\r\n" +//refresh the page automatically every 5 sec
    "\r\n" +
    "<!DOCTYPE HTML>" +
    "<html>" +
    "Analog input:  " + //String(analogRead(A0)) +
    "</html>" +
    "\r\n";
    return htmlPage;
}

void setup() {
  // put your setup code here, to run once:
  //Basic HTML webpage
  page = "<h1>Simple NodeMCU Web Server</h1><p>" +
   "<a href= \"BrewButton\"><button>Brewing Button</button></a>&nbsp;" +
   "<a href=\"LeftButton\"><button>Left</button></a></p>" +
   "<a href=\"RightButton\"><button>Right</button></a></p>";
   "<a href=\"OnOff\"><button>Power Switch (On/Off)</button></a></p>";

  //setting the pins as input or output
  pinMode(BrewLightPin, INPUT_PULLDOWN_16);
  pinMode(BrewButPin, OUTPUT);

  pinMode(BrewLeftPin, OUTPUT);
  pinMode(BrewRightPin, OUTPUT);
  pinMode(OnOffPin, OUTPUT);
  

  //set coffee brewing to low so it's not brewing yet
  digitalWrite(BrewButPin, LOW)

  //Make sure directionals and power switch are low (inactive) until they are used
  digitalWrite(BrewLeftPin, LOW)
  digitalWrite(BrewRightPin, LOW)
  digitalWrite(OnOffPin, LOW)


  
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

//UNIMPLEMENTED

  //When the client requests the left button uri through pressing the left button on the webpage,
  //we simulate the left button press through writing the pin to high, and then low.
  server.on("/LeftButton", [](){
    server.send(200, "text/html", page);
    digitalWrite(BrewLeftPin, HIGH);
    delay(1000);
    digitalWrite(BrewLeftPin, LOW);
  });
  //When the client requests the right button uri through pressing the right button on the webpage,
  //we simulate the right button press through writing the pin to high, and then low.
  
  server.on("/RightButton", [](){
    server.send(200, "text/html", page);
    digitalWrite(BrewRightPin, HIGH);
    delay(1000);
    digitalWrite(BrewRightPin, LOW);
  });

  //client requests on/off uri through pressing power button, leading to us
  //simulating button press through digital signal writing.
  server.on("/OnOff", [](){
    server.send(200, "text/html", page);
    digitalWrite(OnOffPin, HIGH);
    delay(1000);
    digitalWrite(OnOffPin, LOW);
  });

}


  //implement an if to check if the brewing light is on (high?) and set a boolean if it is.
  //use this boolean to confirm or deny the brew button interaction.

void loop() {
  // put your main code here, to run repeatedly:
    server.handleClient ();
}
