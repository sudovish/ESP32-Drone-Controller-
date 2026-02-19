#include <WiFi.h>
#include <WiFiUdp.h>

const char * ssid = "TELLO-FDD299";
const char * password = "";
const char * udpAddress = "192.168.10.1";
const int udpPort = 8889;
boolean connected = false;

int val = 0;

WiFiUDP udp;

void setup() {
  
  Serial.begin(9600);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
}


void loop() {
  String stringOne = "rc 0 0 0 ";
    
    stringOne += val;
    char charBuf[20];
    stringOne.toCharArray(charBuf, 20);
     Serial.println(charBuf);
    
  // read the input on analog pin 0:
  int sensorValue = analogRead(36);

  
  // print out the value you read:
  if(sensorValue<1465){
    int x = 0.068*sensorValue;
    val = 100-x;
    }

    if(sensorValue>=1465 && sensorValue<=2350){
    val = 0;
    }

    if(sensorValue>2350){
      val = -((0.068*sensorValue)-178.46);
      }
  delay(1);
 // delay in between reads for stability
}
