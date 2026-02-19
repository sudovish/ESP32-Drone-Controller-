#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char * networkName = "TELLO-FDD299";
const char * networkPswd = "";

int state = 0; // sets tello into sdk mode
int t = 20; // delay time
int val = 0; // yaw rotation
bool flystate = false; // fly status

String stringval;
String cmd;

const char * udpAddress = "192.168.10.1"; 
const int udpPort = 8889;

boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup() {
  // Initilize hardware serial:
  Serial.begin(115200);

  pinMode(2, OUTPUT); // 26 is button now

  pinMode(25, INPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(27, INPUT);

  pinMode(4, INPUT); // new button
  pinMode(26, INPUT); // takeoff land

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);


}

void loop() {
  bool val1 = digitalRead(25); //down
  bool val2 = digitalRead(32); //left
  bool val3 = digitalRead(33); //up
  bool val4 = digitalRead(27); //right

  bool pwr = digitalRead(26);
  bool dir = digitalRead(4);

  int sensorValue = analogRead(36); //pot
  //only send data when connected

  if (connected && state == 0) {.   //sdk moden on    
    //Send a packet
    udp.beginPacket(udpAddress, udpPort);
    udp.printf("command", millis() / 1000);
    udp.endPacket();
    delay(250);
    state = 1;
  }

  if (connected) {

    if (pwr == HIGH && flystate == false ) { // power sends it flying 
      digitalWrite(2, HIGH);
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("takeoff", millis() / 1000);
      udp.endPacket();
      flystate = true;
    }

    if (pwr == HIGH && flystate == true) { // if already flying then pwr lands drone
      digitalWrite(2, HIGH);
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("land", millis() / 1000);
      udp.endPacket();
      flystate = false;
    }

    delay(1);
  }


  if (dir == HIGH && sensorValue < 1700 && sensorValue > 1525) {  //increase height 
    digitalWrite(2, HIGH);
    udp.beginPacket(udpAddress, udpPort);
    udp.printf("rc 0 0 100 0", millis() / 1000);
    udp.endPacket();
    delay(t);
  }

  if (dir == HIGH && sensorValue > 1700 && sensorValue < 2200) { // reduce height 
    digitalWrite(2, HIGH);
    udp.beginPacket(udpAddress, udpPort);
    udp.printf("rc 0 0 -100 0", millis() / 1000);
    udp.endPacket();
    delay(t);
  }

  //////////////

  if (connected) {

    if (val1 == HIGH) {.       //back
      digitalWrite(2, HIGH);
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("rc 0 -100 0 0", millis() / 1000);
      udp.endPacket();
      delay(t);
    }

    if (val2 == HIGH) { //left
      digitalWrite(2, HIGH);
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("rc -100 0 0 0", millis() / 1000);
      udp.endPacket();
      delay(t);
    }

    if (val3 == HIGH) { //
      digitalWrite(2, HIGH);
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("rc 0 100 0 0", millis() / 1000);
      udp.endPacket();
      delay(t);

    }

    if (val4 == HIGH) {
      digitalWrite(2, HIGH);
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("rc 100 0 0 0", millis() / 1000);
      udp.endPacket();
      delay(t);
    }

    String stringOne = "rc 0 0 0 ";
    stringOne += val;
    char charBuf[20];
    stringOne.toCharArray(charBuf, 20);
    Serial.println(charBuf);

    udp.beginPacket(udpAddress, udpPort);
    udp.printf(charBuf, millis() / 1000);
    udp.endPacket();
    delay(20);

    if (sensorValue < 1465) {
      int x = 0.068 * sensorValue;
      val = 100 - x;
      delay(t);
    }



    if (sensorValue > 2350) {. //yaw limiter and yaw val
      val = -((0.068 * sensorValue) - 178.46);
      delay(t);
    }


    else {.     //default sig
      digitalWrite(2, LOW);
      val = 0;
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("rc 0 0 0 0", millis() / 1000);
      udp.endPacket();
      delay(10);
    }

  }
}

void connectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      //initializes the UDP state
      //This initializes the transfer buffer
      udp.begin(WiFi.localIP(), udpPort);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default: break;
  }
}
