//This example will use a static IP to control the switching of a relay. Over LAN using a web browser. 
//A lot of this code have been resued from the example on the ESP8266 Learning Webpage below. 
//http://www.esp8266learning.com/wemos-webserver-example.php

//CODE START 
//1
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
//#include <PubSubClient.h> 

// Below you will need to use your own WIFI informaiton.
//2

const char* ssid = "your 2.4Gh SSID"; //WIFI Name, WeMo will only connect to a 2.4GHz network.
const char* password = "wifi password"; //WIFI Password

//defining the pin and setting up the "server"
//3
int relayPin = D1; // The Shield uses pin 1 for the relay
int doorPin = D8;
int value1;

WiFiServer server(80);
IPAddress ip(192,168,1,100); // where xx is the desired IP Address
IPAddress gateway(192,168,1,1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network


// void setup is where we initialize variables, pin modes, start using libraries, etc. 
//The setup function will only run once, after each powerup or reset of the wemos board.
//4

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(doorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
   
  Serial.print(F("Setting static ip to : 192.168.1.100"));
  Serial.println(ip);
  
  // Connect to WiFi network
  //5
  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
  //Trying to connect it will display dots
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("waiting to connect...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.println("Use this URL : ");
  Serial.println("http://");
  Serial.println(WiFi.localIP());
  Serial.println("/"); 
  WiFi.mode(WIFI_STA); 
}


void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
 
  int value = LOW;

  if (request.indexOf("/CheckGarage") != -1) {
 
 if (digitalRead(doorPin) == 0) // get new state of door
    value1 = LOW;
    else if (digitalRead(doorPin) == 1)
   value1 = HIGH; 
  } 
  if (request.indexOf("/relay=ON/OFF") != -1){
    digitalWrite(relayPin, HIGH);
    delay(1500);
    digitalWrite(relayPin, LOW);
    value = LOW;
  }
  
  // Return the response, build the html page
  //7
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("#button {");
client.println("background-color: #ffb499;");
client.println("border: none;");
client.println("color: red;");
client.println("padding: 7px 15px;");
client.println("text-align: center;");
client.println("cursor: pointer;");
client.println("}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
  client.print("<meta name ='robots' content ='nofollow'>");

  client.print("<p style='text-align: center;font-size:60px; text-decoration:none'><b>Garage Door Is</b></p>");
 
  if(value1 == LOW) {
    client.print("<p style='text-align: center;font-size:60px;color:red; text-decoration:none'><b>OPENED</b></p>");  
  } else if(value1 == HIGH){
    client.print("<p style='text-align: center;font-size:60px;color:blue; text-decoration:none'><b>CLOSED</b></p>");
  }
  client.println("<br><br><br>");

  client.println("<br><br><br>");
  client.println("<a href=\"/relay=ON/OFF\"><p style='text-align: center;font-size:60px; text-decoration:none'><b>OPEN/CLOSE GARAGE.</b></p></a> <br><br><br>");
  client.println("<a href=\"/CheckGarage\"><p style='text-align: center;font-size:60px; text-decoration:none'><b>CHECK GARAGE</b></p></a><br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 client.println("</body>");
 client.println("</html>");
}//END
