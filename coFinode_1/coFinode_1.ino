#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>   // Include the SPIFFS library

// Replace with your network credentials
const char* ssid = "Myhouse";
const char* password = "redlotus1313";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

//flashLED
const int BrewLidPin = D0;

//IMPLEMENTED

//need to check this pin
const int BrewLightPin = D4;
const int BrewButPin = D5;
const int OnOffPin = D1;

//GPIO3 (D9)for On/Off switch for keurig
// this pin has RXD0
//Using GPIO16 (D0) for Brewing Light Input
//Using GPIO6 (D2) for Brewing Button Output

//flashing light state variables
//based on https://forum.arduino.cc/index.php?topic=101517.0
int flashCounter = 0;   // counter for the number of button presses
int flashState = 0;         // current state of the button
int lastFlashState = 0;     // previous state of the button
int flashing = 0;
int notifySend = 0;





String page  = "";

/*
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
*/


/*
SPIFFS.begin();

  if (!SPIFFS.exists("/formatComplete.txt")) {
    Serial.println("Please wait 30 secs for SPIFFS to be formatted");
    SPIFFS.format();
    Serial.println("Spiffs formatted");
    
    File f = SPIFFS.open("/formatComplete.txt", "w");
    if (!f) {
        Serial.println("file open failed");
    } else {
        f.println("Format Complete");
    }
  } else {
    Serial.println("SPIFFS is formatted. Moving along...");
  }

  if (SPIFFS.exists("/debugLog.txt")){

      File f = SPIFFS.open("/debugLog.txt", "r");
      if (f && f.size()) {
          Serial.println("Dumping log file");
                
          String debugURL = debugLogURL += MAC;
          String debugLogData;
      
          while (f.available()){
            debugLogData += char(f.read());
          }
          f.close();
               
          Serial.println("=====================================");
          Serial.println(debugLogData);
          Serial.println("=====================================");
    
          http.begin(debugURL, fingerprint);
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          int httpCode = http.POST("logData=" + debugLogData);
    //    String response = http.getString();
    //    response.trim();
          Serial.print("Return code = ");  Serial.println(httpCode);
          http.writeToStream(&Serial);
          http.end();

          if (httpCode == 200){
              //Transmitted successfully.  New log.
            Serial.println("Log transmitted successfully.  Truncating...");
            debugLog = SPIFFS.open("/debugLog.txt", "w");
            if (!debugLog) {
                Serial.println("Opening file for write failed");
            }
            debugLog.close();
          } else {
              //Error of some kind - keep the log and append.
              Serial.println("Submission unsuccessful - appending to existing log...");

          }         
      } else {
          Serial.println("Debug log is empty");
      }
      
  }
  SPIFFS.end();
*/
void setup() {
  // put your setup code here, to run once:
  //Basic HTML webpage
  //starting SPI Flash Files System
  SPIFFS.begin();
    String uiresult = "";
  Serial.print("Reading HTML File");  
  if (SPIFFS.exists("/index.txt")){
    File ui = SPIFFS.open("/index.txt", "r");
    while(ui.available()){
      uiresult += char(ui.read());
      Serial.print(uiresult);
    }  
  }
  page = uiresult;
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
  pinMode(BrewLidPin, OUTPUT);

  pinMode(OnOffPin, OUTPUT);

  
  //set coffee brewing to high so it's not brewing yet
  digitalWrite(BrewButPin, HIGH);

  //Make sure directionals and power switch are low (inactive) until they are used
  digitalWrite(OnOffPin, HIGH);

  digitalWrite(BrewLidPin, HIGH);
  
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


  
  Serial.println("Default URI Creation");
  //If server is asking for default uri, send page
  server.on("/", [](){
    server.send(200, "text/html", page);
    Serial.println(" default webpage on");
  });

  
  Serial.println("Brew switch URI Creation");
  //When the client requests the brew button uri through pressing the Brew Button on the webpage,
  //We simulate a brew button press on the keurig through digitally writing the pin to high and then
  //low with a short delay.
  server.on("/BrewButton", [](){
    if(notifySend == 1){
      server.send(200, "text/html", page);
      digitalWrite(BrewButPin, LOW);
      delay(1000);
      digitalWrite(BrewButPin, HIGH);
      notifySend = 0;
    }
    else{
      //Will need to change html or introduce a popup for this statement at a later point
      Serial.println("Cup isn't in place. Cannot succesfully brew.");  
    }
  });

  Serial.println("On/Off switch URI Creation");
  //client requests on/off uri through pressing power button, leading to us
  //simulating button press through digital signal writing.
  server.on("/OnOff", [](){
    server.send(200, "text/html", page);
    digitalWrite(OnOffPin, LOW);
    delay(5000);
    digitalWrite(OnOffPin, HIGH);
    
    delay(5000);

    digitalWrite(BrewLidPin, LOW);
    delay(5000);
    digitalWrite(BrewLidPin, HIGH);
  });
  server.begin();
  Serial.println("Web server started!");

}


  //implement an if to check if the brewing light is on (high?) and set a boolean if it is.
  //use this boolean to confirm or deny the brew button interaction.

void loop(void) {
  // put your main code here, to run repeatedly:
    server.handleClient ();


    //Potentially need to move this
    readStatus();

    if(flashing == 1 && notifySend == 0) {
      Serial.println("flashed, send notification");
      notifySend = 1;
    }

    //if (led full) {     //<----------------- Help?
    //  notifySend = 0;
    //}

}


void readStatus() {
  // read the pushbutton input pin:
  flashState = digitalRead(BrewLightPin);
 
  // compare the buttonState to its previous state
  if (flashState != lastFlashState) {
    // if the state has changed, increment the counter
    if (flashState == HIGH) {
      flashing = 1;
      // if the current state is HIGH then the button
      // went from off to on:
      flashCounter++;
      Serial.println("on");
      Serial.print("Flashes: ");
      Serial.println(flashCounter);
    }
    else {
      // if the current state is LOW then the button
      // went from on to off:
      Serial.println("off");
    }
  }
  // save the current state as the last state,
  //for next time through the loop
  lastFlashState = flashState;
}

