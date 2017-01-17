/*
    Code for individual fuse sensor. 
      1. Sleep until interrupt on GPIO 1
      2. Wake, then connect to AP
      3. Send TCP packet of phase and GPIO state
      4. go back to sleep
*/


#include<ESP8266WiFi.h>

#define GPIO2  2

const char* ssid = "FUSE";
const char* password = "SKUNKWORKS_1!";
const char* host = "192.168.0.2";

bool state = false;

void setup() 
{
  //init serial & GPIO
  Serial.begin(115200);
  pinMode(GPIO2, INPUT_PULLUP);

  //connect to AP
  WiFi.mode(WIFI_STA); //will act as both clinet and server otherwise
  WiFi.begin(ssid,password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  state = digitalRead(GPIO2);
}

WiFiClient cli;

void loop() 
{
  bool newState = digitalRead(GPIO2);
  
  if(newState != state)
  {
    Serial.print("State: "); 
    state = newState;
    Serial.println(state);
    
    if(cli.connect(host,4000)) //should use 4 param?
    {
        if(state)
          cli.println("1");
        else
          cli.println("0");

       /* while(cli.connected())
        {
          if(cli.available())
          {
            String line = cli.readStringUntil('\n');
            Serial.println(line);
          }
        } */
        
        cli.stop();
    }
    else
    {
      Serial.print("Could not connect to host: ");
      Serial.println(host);
    }
    
  }

  delay(500);
}
