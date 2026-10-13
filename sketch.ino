#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int trigPins[] = {23, 19, 17};  
const int echoPins[] = {22, 18, 16};  
const int numSensors = 3;             

float durations[numSensors];
float distances[numSensors];
int sent[numSensors]={0};

//Enable message sending: Send 'I allow callmebot to send me messages' to +34644433804 to receive an API key. Once inserted into the code, the device will start sending messages via WhatsApp."
String phoneNumber = "21620184030"; //put ur phone number here.
String apiKey = "8277889"; //put ur API key here.

void sendMessage(String message,int i) {

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.print("\n Message sent successfully:");
    Serial.print(message);
    sent[i]=abs(sent[i]-1);
  }
  else {
    Serial.println("\nError sending the message");
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\n Connected!");
  for (int i = 0; i < numSensors; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}
void loop() {
  for (int i = 0; i < numSensors; i++) {
    delayMicroseconds(50000);
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);
    
    durations[i] = pulseIn(echoPins[i], HIGH);
    distances[i] = (durations[i] * 0.0343) / 2;
    
    if(distances[i]<50 & sent[i]==0){
      String msg="\n Bin "+String(i)+" needs to be emptied";
      sendMessage(msg,i);
    }
    else if(distances[i]>150 & sent[i]==1){
      String msg="\n Bin "+String(i)+" has been emptied";
      sendMessage(msg,i);
    }

  }
  
  delay(100);
}