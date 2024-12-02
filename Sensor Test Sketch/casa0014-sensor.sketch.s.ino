// This is a test script I used to test whether it would send the mqtt message based on what my sensor was reading
// This is also where i wrote the code to make sure my sensor was working and was printing day/night based on light condtions 
// It worked as I saw the string being printed in serial monitor for both night and day

void setup() {
  Serial.begin(9600);  // open serial port, set the baud rate to 9600 bps
}
void loop() {
  int val;
  val = analogRead(0);  //connect sensor to Analog 0


  Serial.println(val);  //print the value to serial
  delay(1000); //1 second delay 

  if (val > 500) {. // if voltage signal is above 500 it will print day time 
    Serial.println("Daytime");
    sendmqttyellow(); //this will make the chrono lumina yellow
    
    
  } else if (val < 500) { //if voltage signal is below 500 it will print night time 
    Serial.println("Nightime"); 
    sendmqttwhite(); //this will make the chrono lumina white
    
  }
}


void sendmqttyellow(){

  // send a message to update the light
  char mqtt_message[100];
  for (int i = 0; i<12; i++){ //loop that changes the colour of each pixel without me having to repeat it 12 times

  
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 255, \"G\": 154, \"B\": 0, \"W\": 0}", i); //json string sent via mqtt to make chrono lumina yellow
    // Serial.println(mqtt_topic_demo);
    Serial.println(mqtt_message); // prints out the entire string into the serial monitor


//     // if (client.publish(mqtt_topic_demo, mqtt_message)) s{ // I commented out these lines as it wouldnt work without the rest of the main code
//     //   Serial.println("Message published");
//     // }   else {
//     //   Serial.println("Failed to publish message");
//     // }
 }
}

void sendmqttwhite(){

  // send a message to update the light
  char mqtt_message[100];
  for (int i = 0; i<12; i++){

  
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 0, \"G\": 0, \"B\": 0, \"W\": 120}", i); //json string sent via mqtt to make chrono lumina white
    // Serial.println(mqtt_topic_demo);
    Serial.println(mqtt_message);


//     // if (client.publish(mqtt_topic_demo, mqtt_message)) s{
//     //   Serial.println("Message published");
//     // }   else {
//     //   Serial.println("Failed to publish message");
//     // }
 }
}




