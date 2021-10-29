#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#define RXD2 16
#define TXD2 17

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    Serial2.write(0xff);
  }
  
  
  while (true) { 
    WiFiClient client = server.available();
      
      
      if (client.available()) {   
        String req = client.readStringUntil('\r');
        if(req.indexOf("stop") != -1) Serial2.write(0x00);
        if(req.indexOf("forward") != -1) Serial2.write(0x01);
        if(req.indexOf("back") != -1)  Serial2.write(0x02);
        if(req.indexOf("left") != -1) Serial2.write(0x03);
        if(req.indexOf("right") != -1) Serial2.write(0x04);
        if(req.indexOf("leftForward") != -1) Serial2.write(0x05);
        if(req.indexOf("leftBackward") != -1) Serial2.write(0x06);
        if(req.indexOf("rightForward") != -1) Serial2.write(0x07);
        if(req.indexOf("rightBackward") != -1) Serial2.write(0x08);

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println(""); 
        client.flush();
        client.stop();
        Serial.println("Client disconnected.");
    }
  }
}
