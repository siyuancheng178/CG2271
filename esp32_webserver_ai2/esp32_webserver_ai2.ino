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
  
  

  while (true) { 
    WiFiClient client = server.available();
      
      
    if (client.available()) {   
        String req = client.readStringUntil('\r');
        if(req.indexOf("stop") != -1) Serial2.write(0x00);
        else if (req.indexOf("connect") != -1) {
          Serial.write("connect");
          Serial2.write(0xff);
        }
        else if(req.indexOf("forward") != -1) Serial2.write(0x01);
        else if(req.indexOf("back") != -1)  Serial2.write(0x02);
        else if(req.indexOf("Forward") != -1 && req.indexOf("left") != -1) {
          Serial.println("leftForward");
          Serial2.write(0x05);
        }
        else if(req.indexOf("Backward") != -1 && req.indexOf("left") != -1) {
          Serial.println("leftBackward");
          Serial2.write(0x06);
        }
        else if(req.indexOf("Forward") != -1 && req.indexOf("right") != -1) {
          Serial.println("rightForward");
          Serial2.write(0x07);
        }
        else if(req.indexOf("Backward") != -1 && req.indexOf("right") != -1) {
          Serial.println("rightBackward");
          Serial2.write(0x08);
        }
        else if(req.indexOf("left") != -1) Serial2.write(0x03);
        else if(req.indexOf("right") != -1) Serial2.write(0x04);
        

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println(""); 
        client.flush();
        client.stop();
        Serial.println("Client disconnected.");
    }
  }
}
