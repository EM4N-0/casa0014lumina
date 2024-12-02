/* 

Chrono Lumina Blinker example for MKR1010

*/


//#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <WiFiNINA.h>   
#include <PubSubClient.h>
#include <utility/wifi_drv.h>   // library to drive to RGB LED on the MKR1010
#include "arduino_secrets.h" 
#include <Adafruit_NeoPixel.h>
#define pixelPin 6
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, pixelPin);


/*
**** please enter your sensitive data in the Secret tab/arduino_secrets.h
**** using format below

#define SECRET_SSID "ssid name"
#define SECRET_PASS "ssid password"
#define SECRET_MQTTUSER "user name - eg student"
#define SECRET_MQTTPASS "password";
 */

const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiServer server(80);
WiFiClient wificlient;

WiFiClient mkrClient;
PubSubClient client(mkrClient);

// edit this for the light you are connecting to
char mqtt_topic_demo[] = "student/CASA0014/light/10/pixel/";

void setup() {
  // Start the serial monitor to show output
  Serial.begin(115200);
  delay(1000);
  //open serial port, set the baud rate to 9600 bps
  Serial.begin(9600);
  pixels.setBrightness(255);
  pixels.begin();  // Initialize the NeoPixel library
  pixels.clear();  // Turn off all LEDs
  pixels.show();


  WiFi.setHostname("Lumina Eman");
  startWifi();
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("setup complete");
}

void loop() {

  // we need to make sure the arduino is still connected to the MQTT broker
  // otherwise we will not receive any messages
  if (!client.connected()) {
    reconnectMQTT();
  }

  // we also need to make sure we are connected to the wifi
  // otherwise it will be impossible to connect to MQTT!
  if (WiFi.status() != WL_CONNECTED){
    startWifi();
  }

  // check for messages from the broker and ensuring that any outgoing messages are sent.
  client.loop();

  // sendmqtt();
  // Serial.println("sent a message");
  // delay(10000);

  int val;
  int color;
  val = analogRead(0);  //connect sensor to Analog 0


  Serial.println(val);  //print the value to serial
  delay(5000);

  if (val > 500) {
    Serial.println("Daytime"); // If the value is more than 500 it will print day time
    sendmqttyellow(); // This will make the chrono lumina yellow
    color = pixels.Color(255, 154, 0);  // This will make the neopixel led yellow
    for (int i = 0; i < NUMPIXELS; i++) { // This loops means i dont have to run this code 8 times for each pixel
      pixels.setPixelColor(i, color);  // Set each pixel to the selected color
    }
    pixels.show();
    
  } else if (val < 500) { // Same logic as above but if less than 500 it will do the opposite
    Serial.println("Nightime");
    sendmqttwhite();
    color = pixels.Color(255, 255, 255); 

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, color);  
    }
    pixels.show();

    // Delay before taking the next reading
    delay(5000);  // 10-second delay before the next reading
    
  }
  
}

void sendmqtt(){

//   //send a message to update the light
//   char mqtt_message[100];
//   for (int i = 0; i<12; i++){

  
//     sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 255, \"G\": 254, \"B\": 254, \"W\": 0}", i);
    // Serial.println(mqtt_topic_demo);
    // Serial.println(mqtt_message);


//     if (client.publish(mqtt_topic_demo, mqtt_message)) {
//       Serial.println("Message published");
//     }   else {
//       Serial.println("Failed to publish message");
//     }
//  }
}

void sendmqttyellow(){

  // send a message to update the light
  char mqtt_message[100];
  for (int i = 0; i<12; i++){ //Like mentioned previously helps set each pixel without me having to repea the code 12 times 

  
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 255, \"G\": 154, \"B\": 0, \"W\": 0}", i); // json string that sets the colour yellow
    // Serial.println(mqtt_topic_demo);
    Serial.println(mqtt_message); // prints the whole string into the serial monitor


    if (client.publish(mqtt_topic_demo, mqtt_message)) {
      Serial.println("Message published");
    }   else {
      Serial.println("Failed to publish message");
    }
 }
}

void sendmqttwhite(){

  // send a message to update the light
  char mqtt_message[100];
  for (int i = 0; i<12; i++){

  
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 0, \"G\": 0, \"B\": 0, \"W\": 200}", i);
    // Serial.println(mqtt_topic_demo);
    Serial.println(mqtt_message);


    if (client.publish(mqtt_topic_demo, mqtt_message)) {
      Serial.println("Message published");
    }   else {
      Serial.println("Failed to publish message");
    }
 }
}



void startWifi(){
    
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Function for connecting to a WiFi network
  // is looking for UCL_IoT and a back up network (usually a home one!)
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    // loop through all the networks and if you find UCL_IoT or the backup - ssid1
    // then connect to wifi
    Serial.print("Trying to connect to: ");
    Serial.println(ssid);
    for (int i = 0; i < n; ++i){
      String availablessid = WiFi.SSID(i);
      // Primary network
      if (availablessid.equals(ssid)) {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
          delay(600);
          Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("Connected to " + String(ssid));
          break; // Exit the loop if connected
        } else {
          Serial.println("Failed to connect to " + String(ssid));
        }
      } else {
        Serial.print(availablessid);
        Serial.println(" - this network is not in my list");
      }

    }
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED){
    startWifi();
  } else {
    //Serial.println(WiFi.localIP());
  }

  // Loop until we're reconnected
  while (!client.connected()) {    // while not (!) connected....
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "LuminaSelector";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // ... and subscribe to messages on broker
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, int length) {
  // Handle incoming messages
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}