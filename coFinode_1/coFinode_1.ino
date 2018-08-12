#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>   // Include the SPIFFS library

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

//https://forum.arduino.cc/index.php?topic=499399.0
//https://www.arduino.cc/en/Reference/FileRead
//https://forum.arduino.cc/index.php?topic=141418.0
//https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html

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

String readFile(String path){
  String uiresult = "";
  Serial.print("Reading HTML File");  
  if (SPIFFS.exists(path)){
    File ui = SPIFFS.open(path, "r");
    while(ui.available()){
      uiresult += ui.read();
      Serial.print(uiresult);
    }  
  }
}

void setup() {
  // put your setup code here, to run once:
  //Basic HTML webpage
  page = "<h1>Simple NodeMCU Web Server</h1><p> <a href= \"BrewButton\"><button>Brewing Button</button></a>&nbsp; <a href=\"LeftButton\"><button>Left</button></a> <a href=\"RightButton\"><button>Right</button></a> <a href=\"OnOff\"><button>Power Switch (On/Off)</button></a></p>";

  //starting SPI Flash Files System
   SPIFFS.begin();
   /* sample SPIFF code
   String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
   */

  //setting the pins as input or output
  pinMode(BrewLightPin, INPUT_PULLDOWN_16);
  pinMode(BrewButPin, OUTPUT);

  pinMode(BrewLeftPin, OUTPUT);
  pinMode(BrewRightPin, OUTPUT);
  pinMode(OnOffPin, OUTPUT);
  

  //set coffee brewing to low so it's not brewing yet
  digitalWrite(BrewButPin, LOW);

  //Make sure directionals and power switch are low (inactive) until they are used
  digitalWrite(BrewLeftPin, LOW);
  digitalWrite(BrewRightPin, LOW);
  digitalWrite(OnOffPin, LOW);


  
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
