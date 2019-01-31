/*
 * Program for DoIt Tank chasis, project RPM v1.0 (Robotic Pattern Matcher / Robotic Proboscis Monkey) 
 * 
 * Copyright Tomasz Hachaj, 2019
 */
 

/* Create a WiFi access point and provide a web server on it to receive command to control motor. */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "My_Robot";
const char *versionname = "RPM 1.0 (Stanislaw)";

const char *password = "";
const int maxspeed = 1023;
int speedl = maxspeed;
int speedr = maxspeed;

//0 - stop
//1 - forward
//2 - backward
//3 - turn right
//4 - turn left
int motordirection = 0;
 


ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "text/plain", "hello from Robot!");
}

void motor_forward(){
  motordirection = 1;
  /*  analogWrite(1, 1023);
    analogWrite(2, 1023);
    digitalWrite(3, 0);
    digitalWrite(4, 0);
*/
     analogWrite(5, abs(speedl));
     analogWrite(4, abs(speedr));
     digitalWrite(0, 0);
     digitalWrite(2, 0);
  }
void motor_stop(){
  motordirection = 0;
    /*
    analogWrite(1, 0);
    analogWrite(2, 0);
    digitalWrite(3, 1);
    digitalWrite(4, 1);
    */
  digitalWrite(5, 0);
  digitalWrite(4, 0);
  
  digitalWrite(0, 0);
  digitalWrite(2, 0);
  }
void motor_back(){
  motordirection = 2;
    analogWrite(5, abs(speedl));
    analogWrite(4, abs(speedr));
    digitalWrite(0, 1);
    digitalWrite(2, 1);
  }

void motor_left(){
  motordirection = 4;
     analogWrite(5, abs(speedl));
     analogWrite(4, abs(speedr));
    digitalWrite(0, 1);
    digitalWrite(2, 0);
  }
void motor_right(){
  motordirection = 3;
     analogWrite(5, abs(speedl));
     analogWrite(4, abs(speedr));
    digitalWrite(0, 0);
    digitalWrite(2, 1);
  }

  void updatedirection()
  {
    if (motordirection == 0) motor_stop();
    else if (motordirection == 1) motor_forward();
    else if (motordirection == 2) motor_back();
    else if (motordirection == 3) motor_right();
    else if (motordirection == 4) motor_left();
  }

void handlespeed() {
  String message = "";
  if (server.args() > 0)
  {
    float ns = server.arg(0).toFloat();
    if (ns < 0) ns = 0;
    if (ns > 1) ns = 1;
    speedl = (int)(ns * maxspeed);
    speedr = speedl;
    server.send(200, "text/plain", "New speed of left and right motor = " + String(speedl));
    updatedirection();
  }
  server.send(200, "text/plain", "Wrong parameter");
}

void handlespeedl() {
  String message = "";
  if (server.args() > 0)
  {
    float ns = server.arg(0).toFloat();
    if (ns < 0) ns = 0;
    if (ns > 1) ns = 1;
    speedl = (int)(ns * maxspeed);
    server.send(200, "text/plain", "New speed of left motor = " + String(speedl));
    updatedirection();
  }
  server.send(200, "text/plain", "Wrong parameter");
}


void handlespeedr() {
  String message = "";
  if (server.args() > 0)
  {
    float ns = server.arg(0).toFloat();
    if (ns < 0) ns = 0;
    if (ns > 1) ns = 1;
    speedr = (int)(ns * maxspeed);
    server.send(200, "text/plain", "New speed of right motor = " + String(speedl));
    updatedirection();
  }
  server.send(200, "text/plain", "Wrong parameter");
}


void setup() {
  // prepare Motor Output Pins
    /*pinMode(1, OUTPUT); // 1,2EN aka D1 pwm left
    pinMode(2, OUTPUT); // 3,4EN aka D2 pwm right
    pinMode(3, OUTPUT); // 1A,2A aka D3
    pinMode(4, OUTPUT); // 3A,4A aka D4
*/
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(5, 0);
  digitalWrite(4, 0);
  
  digitalWrite(0, 0); //LED Light on
  digitalWrite(2, 0);
  
  delay(1000);
  //Serial.begin(115200);
  //Serial.println();
  //Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
server.begin();
  //IPAddress myIP = WiFi.softAPIP();
  
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);

  //IPAddress ip(192, 168, 1, 101); // where xx is the desired IP Address
  //IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
  //Serial.print(F("Setting static ip to : "));
  //Serial.println(ip);
  //IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  //WiFi.config(ip, gateway, subnet);

 //WiFi.begin(ssid, password);

/*
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }*/
 
  server.on("/", handleRoot);
 /*
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });*/
 
  server.on("/io", []() {
    server.send(200, "text/plain", versionname);
  });

  server.on("/fw", []() {
    motor_forward();
    server.send(200, "text/plain", "Forward");
    //Serial.println("Forward");
  });
  server.on("/bk", []() {
    motor_back();
    server.send(200, "text/plain", "Back");
    //Serial.println("Back");
  });

  server.on("/st", []() {
    motor_stop();
    server.send(200, "text/plain", "Stop");
  });
  server.on("/lt", []() {
    motor_left();
    server.send(200, "text/plain", "Left");
  });
  server.on("/rt", []() {
    motor_right();
    server.send(200, "text/plain", "Right");
  });
  //change speed of left and right motor
  server.on("/speed", handlespeed);
  //change speed of left motor
  server.on("/speedl", handlespeedl);
  //change speed of right motor
  server.on("/speedr", handlespeedr);

  
  //server.onNotFound(handleNotFound);
  server.begin();
  //Serial.println("Start");
  //Serial.println("HTTP server started");


  
}

void loop() {
  /*if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print(".");
    }
  }*/
  
  server.handleClient();
  /*analogWrite(1, 1023);
    analogWrite(2, 1023);
    digitalWrite(3, 0);
    digitalWrite(4, 0);
    delay(200);*/
}
