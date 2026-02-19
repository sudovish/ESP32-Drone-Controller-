#include <WiFi.h>
#include <WiFiUdp.h>

const char * networkName = "TELLO-FDD299";
const char * networkPswd = "";
const char * udpAddress = "192.168.10.1";
const int udpPort = 8889;
boolean connected = false;

WiFiUDP udp;

int state = 0;
int t = 20;
int val=0;






void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  connectToWiFi(networkName, networkPswd);
   
}


void loop() {
 
  int sensorValue = analogRead(36);
   Serial.println(sensorValue);
   if(connected && state==0){
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    udp.printf("command", millis()/1000);
    udp.endPacket();
    delay(250);
    udp.beginPacket(udpAddress,udpPort);
    udp.printf("takeoff", millis()/1000);
    udp.endPacket();
    state=1;
  }
  


  String stringOne = "rc 0 0 0 ";
    
    stringOne += val;
    
    char charBuf[20];
    stringOne.toCharArray(charBuf, 20);
     Serial.println(charBuf);
     
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(charBuf, millis()/1000);
    udp.endPacket();
    delay(20);
  
  if(sensorValue<1465){
    int x = 0.068*sensorValue;
    val = 100-x;
    
    }

    

    else if(sensorValue>2350){
      val = -((0.068*sensorValue)-178.46);
      }
      
    else{
    val = 0;
    }
  delay(1);
  
    
}




















void connectToWiFi(const char * ssid, const char * pwd){
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
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}
