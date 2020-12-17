//This code helps us to send the Humidity and Temperature data measured by DHT11 sensor to ThingSpeak website

#include <DHT.h>  // Library for DTH11 Humidity & Temperature Sensor
#include <ESP8266WiFi.h>   //Library For ESP8266 nodeMCU

#define DHTPIN D3  //ESP8266 pin number
#define DHTTYPE DHT11

const char* ssid="ssid_of_your_network";  //replace with your network SSID
const char* password="password_for_your_network";  //raplace with the passwork for your network
const char* host="api.thingspeak.com";
const char* writeAPIKey="write_API_key"; //This API key is given for your channel in ThingSpeak website

DHT dht(DHTPIN, DHTTYPE,15);

void setup() {
  
  Serial.begin(115200);
  dht.begin();
  delay(1000);
  Serial.println("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("WiFi connected");

}

void loop() {
  // put your main code here, to run repeatedly:
  float hum=dht.readHumidity();
  float temp=dht.readTemperature();

  if(isnan(hum) || isnan(temp))
  {
    return;
  }
  
  WiFiClient client;

  const int httpPort=80;
  if(!client.connect(host,httpPort))
  {
    return;
  }

  String url="/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temp);
  url+="&field2=";
  url+=String(hum);
  url+="\r\n";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.print("Temperature: ");
  Serial.print(temp);

  Serial.print("   ");
  Serial.print("Humidity: ");
  Serial.println(hum);

    client.stop();
    Serial.println("Wait for 15 sec to update datapack in ThingSpeak");
    delay(20000);
}
