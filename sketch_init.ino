#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

// Enter SSID
const char* ssid = "WAGEN";

// String to store app command state
String command;
// Car speed from 400 to 1023
int speedCar = 400;          
int speed_Coeff = 3;

// defines variables
long duration;
int distance;

// Put IP Address details
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

void setup() {
  // Init pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  Serial.begin(9600);

  WiFi.softAP(ssid);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  // Starting WEB-server
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( handle_NotFound );
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  command = server.arg("State");

  Serial.println("Commande: " + command);
  
  // Delivering commands
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "H") goBackLeft();
  else if (command == "0") speedCar = 400;
  else if (command == "1") speedCar = 470;
  else if (command == "2") speedCar = 540;
  else if (command == "3") speedCar = 610;
  else if (command == "4") speedCar = 680;
  else if (command == "5") speedCar = 750;
  else if (command == "6") speedCar = 820;
  else if (command == "7") speedCar = 890;
  else if (command == "8") speedCar = 960;
  else if (command == "9") speedCar = 1023;
  else if (command == "S") stopRobot();
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>CAR Control</title>\n";
  ptr +="<script>\n";
  ptr +="function httpGet(theUrl){var xmlHttp = new XMLHttpRequest();xmlHttp.open( \"GET\", theUrl, false );xmlHttp.send( null );return xmlHttp.responseText;}\n";
  ptr +="function forward(){httpGet(\"http://192.168.1.1?State=F\");}\n";
  ptr +="function backward(){httpGet(\"http://192.168.1.1?State=B\");}\n";
  ptr +="function left(){httpGet(\"http://192.168.1.1?State=L\");}\n";
  ptr +="function right(){httpGet(\"http://192.168.1.1?State=R\");}\n";
  ptr +="function stop(){httpGet(\"http://192.168.1.1?State=S\");}\n";
  ptr +="</script>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; -webkit-touch-callout: none !important;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +="table{width: 100px; margin: auto; font-size: 40px} button{padding: 0px 15px; color: #fff; background-color: #005584; border-radius: 9px; font-size: 40px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body style=\"-moz-user-select: none; -webkit-user-select: none; -ms-user-select:none; user-select:none;-o-user-select:none;\" unselectable=\"on\" onselectstart=\"return false;\" onmousedown=\"return false;\">\n";
  ptr +="<h1>Commande your car</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";

  ptr +="<table>\n";
  ptr +="<tbody>\n";
  ptr +="<tr>\n";
  ptr +="<td align=\"center\"><a href=\"?State=0\">0</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=1\">1</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=2\">2</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=3\">3</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=4\">4</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=5\">5</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=6\">6</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=7\">7</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=8\">8</a></td>\n";
  ptr +="<td align=\"center\"><a href=\"?State=9\">9</a></td>\n";
  ptr +="</tr>\n";
  ptr +="<tr>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="<td>-</td>\n";
  ptr +="</tr>\n";
  ptr +="</tbody>\n";
  ptr +="</table>\n";

  ptr +="<table>\n";
  ptr +="<tbody>\n";
  ptr +="<tr>\n";
  ptr +="<td></td>\n";
  ptr +="<td align=\"center\"><button ontouchstart=\"forward(this)\" ontouchend=\"stop(this)\">&#x2B06;</button></td>\n";
  ptr +="<td></td>\n";
  ptr +="</tr>\n";
  ptr +="<tr>\n";
  ptr +="<td align=\"center\"><button ontouchstart=\"left(this)\" ontouchend=\"stop(this)\">&#x2B05;</button></td>\n";
  ptr +="<td></td>\n";
  ptr +="<td align=\"center\"><button ontouchstart=\"right(this)\" ontouchend=\"stop(this)\">&#x27A1;</button></td>\n";
  ptr +="</tr>\n";
  ptr +="<tr>\n";
  ptr +="<td></td>\n";
  ptr +="<td align=\"center\"><button ontouchstart=\"backward(this)\" ontouchend=\"stop(this)\">&#x2B07;</button></td>\n";
  ptr +="<td></td>\n";
  ptr +="</tr>\n";
  
  ptr +="</tbody>\n";
  ptr +="</table>\n";

  ptr +="<body>\n";
  ptr +="<html>\n";
  
  return ptr;
}

void goAhead() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goRight() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goLeft() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goAheadRight() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goAheadLeft() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar / speed_Coeff);
}

void goBackRight() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goBackLeft() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar / speed_Coeff);
}

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void HTTP_handleRoot(void) {
  if ( server.hasArg("State") ) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", SendHTML()); 
  delay(1);
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
