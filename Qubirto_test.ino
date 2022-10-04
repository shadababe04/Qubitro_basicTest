
#include <QubitroMqttClient.h>
#include <ESP8266WiFi.h>

#define PERIOD 5000

WiFiClient wifiClient;
QubitroMqttClient mqttClient(wifiClient);

// WiFi Credentials
char ssid[] = "Abedin";   
char pass[] = "#Shanam04";

char deviceID[] = "3cd5621b-30b3-447f-93db-a46908590618";
char deviceToken[] = "2nsxzH35UrOq3s-Okf6yg1xzZBkqAq6L6I-G5P6o";
char host[] = "broker.qubitro.com";
int port = 1883;

unsigned long next = 0;

void setup() 
{
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) {;} 
  
  // connect to Wifi network:
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\tConnected to the WiFi !");

  // You need to provide device id and device token
   mqttClient.setId(deviceID);
   mqttClient.setDeviceIdToken(deviceID, deviceToken);

  Serial.println("Connecting to Qubitro...");

  if (!mqttClient.connect(host, port)) 
  {
    Serial.println("Connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    Serial.println("Visit docs.qubitro.com or create a new issue on github.com/qubitro");
    while (1);
  }
  Serial.println("Connected to the Qubitro !");

  mqttClient.onMessage(receivedMessage);    
                                      
  mqttClient.subscribe(deviceID);
}

void loop() 
{
  mqttClient.poll();
  if(millis() > next) {
    next = millis() + PERIOD;
    // Change if possible to have a message over 256 characters
    static char payload[256];
    snprintf(payload, sizeof(payload)-1, "{\"temp\":%d}", 36);
    mqttClient.beginMessage(deviceID);
    // Send value
    mqttClient.print(payload); 
    mqttClient.endMessage();  
  }
}

void receivedMessage(int messageSize) 
{
  Serial.print("New message received:");
  while (mqttClient.available()) 
  {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();
}
